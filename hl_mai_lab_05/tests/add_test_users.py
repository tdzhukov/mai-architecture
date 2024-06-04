import os
import requests

with open(f'{os.path.dirname(__file__)}/users.txt', 'r') as f:
    lines = f.readlines()
    for i in range(1, len(lines)):
        data = lines[i].strip().split(' ')
        params = {
            'first_name': data[0],
            'last_name': data[1],
            'email': data[2],
            'title': data[3],
            'login': data[4],
            'password': data[5],
        }
        response = requests.post('http://localhost:8888/user', params=params, auth=('jsmith1', 'password1'))
        response.raise_for_status()
