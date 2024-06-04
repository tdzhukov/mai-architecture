import os
import requests

token = 'eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpYXQiOjE3MTc1Mjg5MTkuNjY3ODQ2LCJpZCI6MywibG9naW4iOiJ0aW0iLCJzdWIiOiJsb2dpbiJ9.iA-bl9gC6w93SOgT_g6Ovh3RE8Vu_2EusrYIpmqhdBk'
headers = {'Authorization': f'Bearer: {token}'}

with open(f'{os.path.dirname(__file__)}/services.txt', 'r') as f:
    lines = f.readlines()
    for i in range(1, len(lines)):
        data = lines[i].strip().split(' ')
        params = {
            'id': data[0],
            'user_id': data[1],
            'name': data[2],
            'description': data[3],
            'price': data[4],
        }
        response = requests.post('http://localhost:8888/service', params=params, headers=headers)
        response.raise_for_status()

with open(f'{os.path.dirname(__file__)}/orders.txt', 'r') as f:
    lines = f.readlines()
    for i in range(1, len(lines)):
        data = lines[i].strip().split(' ')
        params = {
            'id': data[0],
            'user_id': data[1],
            'address': data[2],
            'service_ids': data[3],
        }
        response = requests.post('http://localhost:8888/order', params=params, headers=headers)
        response.raise_for_status()
