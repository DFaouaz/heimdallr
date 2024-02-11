#!/bin/bash

proyecto="$1"
archivos_a_agregar=("${@:2}")

if [ ! -f "$proyecto" ]; then
  echo "El archivo del proyecto '$proyecto' no existe."
  exit 1
fi

# Función para escapar caracteres especiales XML
function escape_xml {
  echo "$1" | sed 's/&/\&amp;/g; s/</\&lt;/g; s/>/\&gt;/g; s/"/\&quot;/g; s/'\''/\&apos;/g'
}

# Agregar archivos al proyecto
for archivo in "${archivos_a_agregar[@]}"; do
  # Verificar si el archivo ya está en el proyecto
  archivo_existente=$(grep -o -P "<ClCompile Include=\"$(escape_xml "$archivo")\" />" "$proyecto")

  if [ -z "$archivo_existente" ]; then
    # Crear un nuevo elemento ClCompile
    nuevo_elemento="<ItemGroup><ClCompile Include=\"$(escape_xml "$archivo")\" /><\/ItemGroup>"
    sed -i "/<\/Project>/i $nuevo_elemento" "$proyecto"
    echo "Agregando '$archivo' al proyecto."
  else
    echo "El archivo '$archivo' ya está en el proyecto."
  fi
done

echo "Proyecto actualizado exitosamente."
exit 0