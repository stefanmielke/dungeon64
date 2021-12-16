using ldtk;

T GetValue<T>(IEnumerable<FieldInstance> fields, string fieldName)
{
    return (T) fields.First(fi => fi.Identifier == fieldName).Value;
}

if (args.Length < 3)
{
    Console.WriteLine(
        "usage: dotnet mapbuilder path/to/map_template.txt path/to/map/folder path/to/header/output/folder");
    return -1;
}

var mapTemplateFile = args[0];
var inputFolder = args[1];
var outputFolder = args[2];

Console.ForegroundColor = ConsoleColor.Gray;
Console.WriteLine("Map Template: {0}", mapTemplateFile);
Console.ResetColor();
Console.ForegroundColor = ConsoleColor.Gray;
Console.WriteLine("Input folder: {0}", inputFolder);
Console.ResetColor();
Console.ForegroundColor = ConsoleColor.Gray;
Console.WriteLine("Output folder: {0}", outputFolder);
Console.ResetColor();

var baseFileTemplate = File.ReadAllText(mapTemplateFile);
var filesCreated = new Dictionary<int, string>();

foreach (var filename in Directory.GetFiles(inputFolder, "*.ldtk", SearchOption.AllDirectories))
{
    Console.ForegroundColor = ConsoleColor.Green;
    Console.WriteLine("File found: {0}", filename);
    Console.ResetColor();

    var ldtkJson = LdtkJson.FromJson(File.ReadAllText(filename));
    foreach (var level in ldtkJson.Levels)
    {
        var mapName = level.Identifier?.ToLower() ?? "NoName";
        var mapId = GetValue<int>(level.FieldInstances, "ID");
        var stepsToCombatStart = GetValue<int>(level.FieldInstances, "STEPS_TO_COMBAT_START");
        var stepsToCombatEnd = GetValue<int>(level.FieldInstances, "STEPS_TO_COMBAT_END");
        var hasRandomEncounters = GetValue<bool>(level.FieldInstances, "HAS_RANDOM_ENCOUNTERS");
        var spriteGround = GetValue<string>(level.FieldInstances, "SPRITE_GROUND");
        var spriteWall = GetValue<string>(level.FieldInstances, "SPRITE_WALL");
        var spriteWallExit = GetValue<string>(level.FieldInstances, "SPRITE_WALL_EXIT");
        var spriteWallUpstairs = GetValue<string>(level.FieldInstances, "SPRITE_WALL_UPSTAIRS");

        var tilesLayer = level.LayerInstances.First(li => li.Identifier == "Tiles");
        var mapWidth = tilesLayer.CWid;
        var mapSize = mapWidth * tilesLayer.CHei;
        var gridSize = tilesLayer.GridSize;

        var mapTiles = new long[mapSize];

        foreach (var tile in tilesLayer.AutoLayerTiles)
        {
            var x = tile.Px[0] / gridSize;
            var y = tile.Px[1] / gridSize;
            var pos = x + (y * mapWidth);
            mapTiles[pos] = tile.T;
        }

        var entitiesLayer = level.LayerInstances.First(li => li.Identifier == "Entities");

        var entities = new List<Event>(entitiesLayer.EntityInstances.Length);
        foreach (var entity in entitiesLayer.EntityInstances)
        {
            var tilePosition = entity.Grid[0] + (entity.Grid[1] * mapWidth);
            var type = entity.Identifier;

            var entityEvent = new Event(type, tilePosition);
            switch (type)
            {
                case "MET_Spawn":
                    foreach (var arg in entity.FieldInstances)
                        entityEvent.Args.Add("args.spawn." + arg.Identifier, arg.Value.ToString());
                    break;
                case "MET_Stairs":
                    foreach (var arg in entity.FieldInstances)
                        entityEvent.Args.Add("args.stairs." + arg.Identifier, arg.Value.ToString());
                    break;
                case "MET_Exit":
                default:
                    break;
            }

            entities.Add(entityEvent);
        }

        string fileString = new string(baseFileTemplate);
        fileString = fileString.Replace("${MAP_NAME}", mapName);
        fileString = fileString.Replace("${STEPS_TO_COMBAT_START}", stepsToCombatStart.ToString());
        fileString = fileString.Replace("${STEPS_TO_COMBAT_END}", stepsToCombatEnd.ToString());
        fileString = fileString.Replace("${HAS_RANDOM_ENCOUNTERS}", hasRandomEncounters.ToString().ToLower());
        fileString = fileString.Replace("${SPRITE_GROUND}", spriteGround);
        fileString = fileString.Replace("${SPRITE_WALL}", spriteWall);
        fileString = fileString.Replace("${SPRITE_WALL_EXIT}", spriteWallExit);
        fileString = fileString.Replace("${SPRITE_WALL_UPSTAIRS}", spriteWallUpstairs);
        fileString = fileString.Replace("${MAP_SIZE}", mapSize.ToString());
        fileString = fileString.Replace("${MAP_WIDTH}", mapWidth.ToString());
        fileString = fileString.Replace("${MAP_TILES}", string.Join(",", mapTiles.Select(t => t.ToString())));
        fileString = fileString.Replace("${EVENTS_COUNT}", entities.Count.ToString());
        fileString = fileString.Replace("${EVENTS_DATA}", string.Join("", entities.Select((e, i) => e.ToString(i))));

        var filenameCreated = mapName + ".gen.h";
        filesCreated.Add(mapId, filenameCreated);

        var generatedFilename = Path.Combine(outputFolder, filenameCreated);
        File.WriteAllText(generatedFilename, fileString);

        Console.ForegroundColor = ConsoleColor.Green;
        Console.WriteLine("Wrote file: {0}", generatedFilename);
        Console.ResetColor();
    }

    var functionMapsCFile = string.Concat(@"
MapDef *map_get_def_by_id(u16 map_id) {
    switch (map_id) {
",
        string.Join("\n", filesCreated.Select((f) => $"\t\tcase {f.Key}:\n\t\t\treturn &{f.Value.Replace(".gen.h", "")};"))
        , @"
        default:
            return NULL;
    }
}");

    var mapsCFile = string.Join("\n", filesCreated.Select(f => $"#include \"{f.Value}\""));
    var mapsHFile = "#pragma once\n\n#include \"map_def.h\"\n\n" +
                    string.Join("\n",
                        filesCreated.Select(f =>
                            $"extern MapDef {f.Value.Replace(".gen.h", "")};"));

    File.WriteAllText(Path.Combine(outputFolder, "maps.gen.c"), mapsCFile + $"\n{functionMapsCFile}");
    File.WriteAllText(Path.Combine(outputFolder, "maps.gen.h"),
        mapsHFile + "\n\nMapDef *map_get_def_by_id(u16 map_id);");

    Console.ResetColor();
}

return 0;

internal class Event
{
    public Event(string type, long tilePosition)
    {
        Type = type;
        TilePosition = tilePosition;
        Args = new Dictionary<string, string>();
    }

    public string Type { get; }
    public long TilePosition { get; }
    public Dictionary<string, string> Args { get; }

    public string ToString(int index)
    {
        var args = string.Join("", Args.Select(a => $"[{index}].{a.Key} = {a.Value},\n"));

        return $"[{index}].type = {Type},\n[{index}].tile_position = {TilePosition},\n{args}";
    }
}