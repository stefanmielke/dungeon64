mkdir -p src/res
mkdir -p src/res/gfx/enemies
mkdir -p src/res/gfx/font
mkdir -p src/res/gfx/player
mkdir -p src/res/gfx/textures
mkdir -p src/res/gfx/ui

for n in $(find resources/gfx/enemies/ -name '*.png'); do
    from="$n"
    to=$(echo "$n" | sed "s/resources/src\/res/g; s/png/gen.h/g")
    n64graphics -i $to -g $from -s u16 -f rgba16 -w 64 -h 64
done
for n in $(find resources/gfx/font/ -name '*.png'); do
    from="$n"
    to=$(echo "$n" | sed "s/resources/src\/res/g; s/png/gen.h/g")
    to_palette=$(echo "$n" | sed "s/resources/src\/res/g; s/.png/_palette.gen.h/g")
    n64graphics -i $to -g $from -p $to_palette -s u16 -f ci4 -w 64 -h 64
done
for n in $(find resources/gfx/player/ -name '*.png'); do
    from="$n"
    to=$(echo "$n" | sed "s/resources/src\/res/g; s/png/gen.h/g")
    n64graphics -i $to -g $from -s u16 -f rgba16 -w 64 -h 64
done
for n in $(find resources/gfx/textures/ -name '*.png'); do
    from="$n"
    to=$(echo "$n" | sed "s/resources/src\/res/g; s/png/gen.h/g")
    n64graphics -i $to -g $from -s u16 -f rgba16 -w 32 -h 32
done
for n in $(find resources/gfx/ui/ -name '*.png'); do
    from="$n"
    to=$(echo "$n" | sed "s/resources/src\/res/g; s/png/gen.h/g")
    n64graphics -i $to -g $from -s u16 -f rgba16 -w 48 -h 36
done

dotnet run --project tools/map-builder/MapBuilder/MapBuilder.csproj tools/map-builder/MapBuilder/map_template.txt resources/maps src/maps/