using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Networking;
using System.Net.WebSockets;
using System.Text;
using System.Threading;
using System;
using System.Threading.Tasks;
using UnityEditor;
using UnityEditor.VersionControl;

public class Main : MonoBehaviour
{
    private ClientWebSocket _webSocket;
    public string serverUri = "ws://localhost:8765";
    public string clientId = "";

    public delegate void Connect();
    public static event Connect OnConnect;

    public delegate void Receive(string info);
    public static event Receive OnDataReceive;

    public static int Status;

    void Start()
    {
        
    }

    public async void ConnectToServer(string id)
    {
        clientId = id;

        _webSocket = new ClientWebSocket();
        await _webSocket.ConnectAsync(new System.Uri(serverUri), CancellationToken.None);

        OnConnect?.Invoke();
        await WSSendMessage(clientId);

        StartCoroutine(ReceiveMessages());
    }

    private async System.Threading.Tasks.Task WSSendMessage(string message)
    {
        if (_webSocket.State == WebSocketState.Open)
        {
            byte[] bytes = Encoding.UTF8.GetBytes(message);
            await _webSocket.SendAsync(new System.ArraySegment<byte>(bytes), WebSocketMessageType.Text, true, CancellationToken.None);
        }
    }

    private IEnumerator ReceiveMessages()
    {
        byte[] buffer = new byte[1024];

        while (_webSocket.State == WebSocketState.Open)
        {
            var receiveTask = _webSocket.ReceiveAsync(new System.ArraySegment<byte>(buffer), CancellationToken.None);

            yield return new WaitUntil (() => receiveTask.IsCompleted);

            if (receiveTask.Result.MessageType == WebSocketMessageType.Text)
            {
                string message = Encoding.UTF8.GetString(buffer, 0, receiveTask.Result.Count);
                Debug.Log($"Message from server: {message}");
                OnDataReceive?.Invoke(message);
            }
            else if (receiveTask.Result.MessageType == WebSocketMessageType.Close)
            {
                Debug.Log("WebSocket connection closed by the server.");
                OnDataReceive?.Invoke("error");
                yield break;
            }
        }
    }

    private async void OnApplicationQuit()
    {
        if (_webSocket != null)
        {
            await _webSocket.CloseAsync(WebSocketCloseStatus.NormalClosure, "Closing", CancellationToken.None);
            _webSocket.Dispose();
        }
    }

}
