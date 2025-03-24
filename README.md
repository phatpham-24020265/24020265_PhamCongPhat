# CÁCH CHƠI GAME:
## Cài đặt:
- Cài codeblocks-20.03mingw-setup.exe
- Cài 4 file sau : SDL2_image-2.8.5, SDL2_mixer-2.8.1, SDL2_ttf-devel-2.24.0-mingw, SDL2-devel-2.32.0-mingw. (Lưu ý phiên bản)

## Set up:
- Copy file dll vào thư mục mã nguồn project (nơi có các file .cpp)
…..\x86_64-w64-mingw32\bin\SDL2.dll 
- Setting | Compiler | Linker Setting: chép vào Other Linker Option: -lmingw32 -lSDL2main -lSDL2
- Setting | Compiler | SearchDirectory | Compiler: thêm vào Policy đường dẫn: …..\x86_64-w64-mingw32\include\SDL2 
- Setting | Compiler | SearchDirectory | Linker: thêm vào Policy đường dẫn:…..\x86_64-w64-mingw32\lib
- Làm tương tự với SDL2 image, mixer, ttf.

## Tải code về và trải nghiệm.
 
