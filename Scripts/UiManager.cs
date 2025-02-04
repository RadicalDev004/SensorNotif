using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class UiManager : MonoBehaviour
{
    public Main main;

    [Header("Input code")]
    public GameObject InputCodeObj;
    public TMP_InputField In_Code;
    public GameObject Loading;

    [Header("Status")]
    public Color red, green;
    public Image I_Status;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void TakeInputCode()
    {
        main.ConnectToServer(In_Code.text);
        In_Code.text = string.Empty;

        InputCodeObj.SetActive(false);
        Loading.SetActive(true);
    }

    public void DoOnConnect()
    {
        Loading.SetActive(false);
    }

    public void DoOnReceiveData(string info)
    {
        if (info == "error")
            return;

        string[] arr = info.Split('-');
        int status = 0;

        if (arr[0] == "sensor")
            status = int.Parse(arr[1]);

        I_Status.color = status == 1 ? Color.green : Color.red;
    }

    private void OnEnable()
    {
        Main.OnConnect += DoOnConnect;
        Main.OnDataReceive += DoOnReceiveData;
    }

    private void OnDisable()
    {
        Main.OnConnect -= DoOnConnect;
        Main.OnDataReceive -= DoOnReceiveData;
    }
}
