from flask import Flask, render_template, request
#from fastapi import FastAPI, HTTPException, UploadFile, File
from db import DataBase

data = DataBase()
app = Flask(__name__)

@app.route('/')
def map():
    return render_template('map.html')

@app.route('/upload', methods=['GET','POST'])
def search():
    if request.method == 'GET':
        return render_template('upload.html')   
    else:
        try:
            file = request.files['file']
            content = file.read()          # bytes
            content = content.decode('utf-8') # si es texto
            for line in content.split('\n'):
                try:
                    data.save_network(line.strip())
                except Exception as e:
                    print(e)
            
            return {"status":0}
        except Exception as e:
            return {"status":-1}

app.run(host="0.0.0.0")