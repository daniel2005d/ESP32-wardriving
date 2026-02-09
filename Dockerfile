FROM python:3.11-slim

# Instalar git
#RUN apt-get update && apt-get install -y libglib2.0-0 libgl1 python3-opencv python3-pip

# Instalar dependencias
WORKDIR /app
COPY ESP32-wardriving/site/requirements.txt ./requirements.txt
RUN pip install --no-cache-dir -r requirements.txt
#RUN pip install --no-cache-dir fastapi==0.111.0 pydantic==2.6.4 motor==3.3.1 pymongo==4.6.3 numpy==1.24.2 opencv_python==4.11.0.86 requests Pillow==11.3.0 aiogram tflite-runtime numpy rapidfuzz cryptography colored psutil bs4

# Ejecutar la aplicación
#CMD ["uvicorn", "api.main:app", "--host", "0.0.0.0", "--port", "8000"]
