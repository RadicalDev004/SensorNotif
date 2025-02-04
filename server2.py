from flask import Flask, request, jsonify
from flask_socketio import SocketIO, emit

app = Flask(__name__)
socketio = SocketIO(app, cors_allowed_origins="*")  # Enable WebSocket support and allow Unity connections

connected_clients = {}

# HTTP endpoint for ESP32 uploads
@app.route('/upload', methods=['POST'])
def upload_data():
    data = request.json
    print(f"Received data: {data}")

    # Extract device_id from ESP32 data
    device_id = data.get('device_id')
    payload = data.get('payload')

    # If a Unity client is connected for this device_id, send the data
    for client_id, client_device_id in connected_clients.items():
        if client_device_id == device_id:
            socketio.emit('device_data', payload, to=client_id)
            print(f"Sent data to Unity client with device_id {device_id}")
            break

    return jsonify({'status': 'success', 'message': 'Data processed'})

# WebSocket endpoint for Unity clients
@socketio.on('connect', namespace='/ws')
def handle_connect():
    print("A Unity client connected.")

@socketio.on('disconnect', namespace='/ws')
def handle_disconnect():
    client_id = request.sid
    connected_clients.pop(client_id, None)
    print(f"Unity client {client_id} disconnected.")

@socketio.on('join', namespace='/ws')
def handle_join(data):
    # Unity client sends its device_id upon joining
    device_id = data.get('device_id')
    client_id = request.sid

    # Store the mapping of client_id to device_id
    connected_clients[client_id] = device_id
    print(f"Unity client {client_id} registered with device_id {device_id}")

if __name__ == "__main__":
    socketio.run(app, host="0.0.0.0", port=5001)