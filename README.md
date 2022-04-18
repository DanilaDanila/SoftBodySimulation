# SoftBody

Маленькая и страшная библиотечка для симуляции физики мягкого тела.

## Build

### Unix / Linux

1. Поставьте пакет `sdl2`

2. Дальше всё просто и очевидно

   ```{bash}
   $ mkdir build
   $ cd build
   $ cmake ..
   $ make
   ```

### Windows

1. Переходим на сайт [sdl2](https://www.libsdl.org/download-2.0.php)

2. Скачиваем "Development Libraries" для нужного компилятора

3. Открываем CMakeLists.txt и прописываем пути до папок `include` и `lib`
   Например так:

   ```{cmake}
   set(SDL2_INCLUDE_DIRS C:/Users/danila/Downloads/SDL2-devel-2.0.20-mingw.tar/SDL2-devel-2.0.20-mingw/SDL2-2.0.20/x86_64-w64-mingw32/include)
   set(SDL2_LIBRARIES C:/Users/danila/Downloads/SDL2-devel-2.0.20-mingw.tar/SDL2-devel-2.0.20-mingw/SDL2-2.0.20/x86_64-w64-mingw32/lib)
   ```

4. Запускаем сборку

   ```{bash}
   > mkdir build
   > cd build
   > cmake .. -G"тут нужную систему сборки"
   > make
   ```

5. После сборки находим файл `SDL2-devel-2.0.20-mingw/SDL2-2.0.20/x86_64-w64-mingw32/bin/SDL2.dll` и копируем его в папку `build` РЯДОМ С ФАЙЛОМ `sandbox.exe

