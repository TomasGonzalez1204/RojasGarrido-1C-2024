# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/Jesse/esp/v5.1.2/esp-idf/components/bootloader/subproject"
  "C:/Repositorios/RojasGarrido-1C-2024/firmware/projects/Guia1_ej3/build/bootloader"
  "C:/Repositorios/RojasGarrido-1C-2024/firmware/projects/Guia1_ej3/build/bootloader-prefix"
  "C:/Repositorios/RojasGarrido-1C-2024/firmware/projects/Guia1_ej3/build/bootloader-prefix/tmp"
  "C:/Repositorios/RojasGarrido-1C-2024/firmware/projects/Guia1_ej3/build/bootloader-prefix/src/bootloader-stamp"
  "C:/Repositorios/RojasGarrido-1C-2024/firmware/projects/Guia1_ej3/build/bootloader-prefix/src"
  "C:/Repositorios/RojasGarrido-1C-2024/firmware/projects/Guia1_ej3/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Repositorios/RojasGarrido-1C-2024/firmware/projects/Guia1_ej3/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Repositorios/RojasGarrido-1C-2024/firmware/projects/Guia1_ej3/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
