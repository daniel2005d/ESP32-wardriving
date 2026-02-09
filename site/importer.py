from db import DataBase

data = DataBase()

with open('/mnt/hgfs/Proyectos/coordinates.txt', 'r') as f:
    for line in f.readlines():
        try:
            data.save_network(line.strip())
        except Exception as e:
            print(e)

