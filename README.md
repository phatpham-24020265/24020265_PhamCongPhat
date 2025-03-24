# CÁCH CHƠI GAME:
- Cài codeblocks-20.03mingw-setup.exe
- Cài 4 file sau : SDL2_image-2.8.5, SDL2_mixer-2.8.1, SDL2_ttf-devel-2.24.0-mingw, SDL2-devel-2.32.0-mingw. (Lưu ý phiên bản)
- Sau đó trong phần Setting chọn Compiler ở phần Linker settings - Other linker options, điền -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer.
- Tiếp theo trong phần Search directories - Complier
