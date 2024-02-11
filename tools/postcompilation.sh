# TODO: sin hacer, esto no sirve de nada
# Definir la ruta al directorio donde buscar archivos .generated.cpp
ruta_directorio="../Sandbox/"

proyecto_original="Sandbox.vcxproj"
nombre_backup="Sandbox.vcxproj.backup"

if [ -f "$ruta_directorio$nombre_backup" ]; then
  cp "$ruta_directorio$proyecto_original" "$ruta_directorio$nombre_backup"
  echo "El archivo '$proyecto_original' ha sido copiado como '$nombre_backup'."
else
  echo "El archivo '$proyecto_original' no existe."
  exit 1
fi

# Buscar todos los archivos con la extensión .generated.cpp en la ruta especificada
archivos_generated_cpp=($(find "$ruta_directorio" -type f -name "*.heimdallr_generated.cpp"))

# Verificar si se encontraron archivos
if [ ${#archivos_generated_cpp[@]} -eq 0 ]; then
  echo "No se encontraron archivos con la extensión .generated.cpp en '$ruta_directorio'."
  exit 1
fi

# Llamar al script y pasar los archivos encontrados como argumentos
./add_generated_files.sh "../Sandbox/Sandbox.vcxproj" "${archivos_generated_cpp[@]}"