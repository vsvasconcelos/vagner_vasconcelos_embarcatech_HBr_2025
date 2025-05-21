# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/home/vagner/pico/vagner_vasconcelos_embarcatech_HBr_2025/projetos/seguranca_iot/build/_deps/picotool-src")
  file(MAKE_DIRECTORY "/home/vagner/pico/vagner_vasconcelos_embarcatech_HBr_2025/projetos/seguranca_iot/build/_deps/picotool-src")
endif()
file(MAKE_DIRECTORY
  "/home/vagner/pico/vagner_vasconcelos_embarcatech_HBr_2025/projetos/seguranca_iot/build/_deps/picotool-build"
  "/home/vagner/pico/vagner_vasconcelos_embarcatech_HBr_2025/projetos/seguranca_iot/build/_deps"
  "/home/vagner/pico/vagner_vasconcelos_embarcatech_HBr_2025/projetos/seguranca_iot/build/picotool/tmp"
  "/home/vagner/pico/vagner_vasconcelos_embarcatech_HBr_2025/projetos/seguranca_iot/build/picotool/src/picotoolBuild-stamp"
  "/home/vagner/pico/vagner_vasconcelos_embarcatech_HBr_2025/projetos/seguranca_iot/build/picotool/src"
  "/home/vagner/pico/vagner_vasconcelos_embarcatech_HBr_2025/projetos/seguranca_iot/build/picotool/src/picotoolBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/vagner/pico/vagner_vasconcelos_embarcatech_HBr_2025/projetos/seguranca_iot/build/picotool/src/picotoolBuild-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/vagner/pico/vagner_vasconcelos_embarcatech_HBr_2025/projetos/seguranca_iot/build/picotool/src/picotoolBuild-stamp${cfgdir}") # cfgdir has leading slash
endif()
