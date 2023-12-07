if [ -z "$CC" ]
then
	CC=gcc
fi
if [ -z "$KEYMAP" ]
then
	KEYMAP=KEYMAP00
fi
$CC -D$KEYMAP=1 ./src/*.c -o ./bin/sdlman -lSDL_image  -ggdb -lSDL -lSDL_ttf -lm
cp -r -p ./res/* ./bin/
