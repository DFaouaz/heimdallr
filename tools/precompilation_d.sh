# Definir la ruta al directorio donde buscar archivos .generated.cpp
ruta_directorio="../Sandbox/"
proyecto_original="Sandbox.vcxproj"
nombre_backup="Sandbox.vcxproj.backup"

if [ -f "$ruta_directorio$proyecto_original" ]; then
  cp "$ruta_directorio$proyecto_original" "$ruta_directorio$nombre_backup"
  echo "El archivo '$proyecto_original' ha sido copiado como '$nombre_backup'."
else
  echo "El archivo '$proyecto_original' no existe."
  exit 1
fi

# Buscar todos los cpps para compilar
find "$ruta_directorio" -type f -name "*.cpp" | grep -v "generated.cpp" | while read -r archivo_cpp; do
  echo "$archivo_cpp"
  "../bin/Heimdallr/x64/Release/Heimdallr_x64.exe" "$archivo_cpp" -- -D__GENERATE_HEIMDALLR_REFLECTION_ANNOTATION__ -D_DEBUG -D_UNICODE -DUNICODE -fparse-all-comments -x c++ -std=c++17 -I"C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.34.31933/include" -I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\ucrt" -I../Heimdallr/src/
done

# Buscar todos los archivos con la extensión .generated.cpp en la ruta especificada
archivos_generated_cpp=($(find "$ruta_directorio" -type f -name "*.heimdallr_generated.cpp"))

# Verificar si se encontraron archivos
if [ ${#archivos_generated_cpp[@]} -eq 0 ]; then
  echo "No se encontraron archivos con la extensión .generated.cpp en '$ruta_directorio'."
  exit 1
fi

# Llamar al script y pasar los archivos encontrados como argumentos
bash ./add_generated_files.sh "../Sandbox/Sandbox.vcxproj" "${archivos_generated_cpp[@]}" 

msbuild $ruta_directorio$proyecto_original /t:Reload