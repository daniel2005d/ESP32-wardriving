from flask import Flask, render_template, request
#from fastapi import FastAPI, HTTPException, UploadFile, File
from db import DataBase

data = DataBase()
app = Flask(__name__)

@app.route('/')
def map():
    return render_template('map.html', points='40,50/20,30')

@app.route('/upload', methods=['GET','POST'])
def search():
    if request.method == 'GET':
        return render_template('upload.html')
    else:
        try:
            file = request.files['file']
            content = file.read()          # bytes
            positions = []
            content = content.decode('utf-8') # si es texto
            for line in content.split('\n'):
                try:
                    point = data.save_network(line.strip())
                    if point and point not in positions:
                        positions.append(point)
                except Exception as e:
                    print(e)
            
            return render_template('map.html', points='/'.join(positions))
        except Exception as e:
            return {"status":-1}


app.debug=True
app.run(host="0.0.0.0")