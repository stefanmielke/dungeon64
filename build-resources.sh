mkdir -p src/res
mkdir -p src/res/gfx/enemies
mkdir -p src/res/gfx/player
mkdir -p src/res/gfx/textures
mkdir -p src/res/gfx/ui

for n in $(find resources/gfx/enemies/ -name '*.png'); do
    from="$n"
    to=$(echo "$n" | sed "s/resources/src\/res/g; s/png/gen.h/g")
    n64graphics -i $to -g $from -s u16 -f rgba16 -w 64 -h 64
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