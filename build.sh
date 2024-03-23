if [ -z "$CC" ]
then
	CC=gcc
fi
if [ -z "$KEY" ]
then
	KEY=KEYMAP_00
fi
COPT="-D $KEY"
mkdir -p bin
$CC -O3 $COPT ./src/*.c -o ./bin/sdlman -lSDL_image  -ggdb -lSDL -lSDL_ttf -lm
echo "$CC $COPT ./src/*.c -o ./bin/sdlman -lSDL_image  -ggdb -lSDL -lSDL_ttf -lm"
cp -r -p ./res/* ./bin/
