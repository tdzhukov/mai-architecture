import os
import requests

with open(f'{os.path.dirname(__file__)}/services.txt', 'r') as f:
    lines = f.readlines()
    for i in range(1, len(lines)):
        data = lines[i].split(' ')
        files = {
            ('id', (None, data[0])),
            ('user_id', (None, data[1])),
            ('name', (None, data[2])),
            ('description', (None, data[3])),
            ('price', (None, data[4])),
        }
        response = requests.post('http://localhost:8080/service', files=files)
        response.raise_for_status()

with open(f'{os.path.dirname(__file__)}/orders.txt', 'r') as f:
    lines = f.readlines()
    for i in range(1, len(lines)):
        data = lines[i].split(' ')
        files = {
            ('id', (None, data[0])),
            ('user_id', (None, data[1])),
            ('address', (None, data[2])),
            ('service_ids', (None, data[3])),
        }
        response = requests.post('http://localhost:8080/order', files=files)
        response.raise_for_status()
