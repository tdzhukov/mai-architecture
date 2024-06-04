import os
import requests

with open(f'{os.path.dirname(__file__)}/users.txt', 'r') as f:
    lines = f.readlines()
    for i in range(1, len(lines)):
        data = lines[i].split(' ')
        files = {
            ('first_name', (None, data[0])),
            ('last_name', (None, data[1])),
            ('email', (None, data[2])),
            ('title', (None, data[3])),
            ('login', (None, data[4])),
            ('password', (None, data[5])),
        }
        response = requests.post('http://localhost:8080/user', files=files)
        response.raise_for_status()
