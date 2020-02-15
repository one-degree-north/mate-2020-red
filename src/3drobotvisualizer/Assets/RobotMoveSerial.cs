using System.Collections;
using System.Collections.Generic;
using System;
using System.IO.Ports;
using System.Threading;
using UnityEngine;
using UnityEngine.UI;

public class RobotMoveSerial : MonoBehaviour
{

	public Transform rov;
	public Vector3 a;
	public Vector3 g;
	public int temp;
	//public Text tempText;

	static SerialPort stream;

    // Start is called before the first frame update
    void Start()
    {
        stream = new SerialPort();
        stream.PortName = "COM5";
        stream.BaudRate = 115200;
        stream.ReadTimeout = 200;
        stream.Open();

    }

    // Update is called once per frame
    //void Update()
    //{
        
    //}

    void Update()
    {
    	string data = readData(200);
    	if (data != null) {
	    	string header = data.Substring(0, 3);
	    	string message = data.Substring(3, data.Length - 3);
	    	print(header);
	    	print(message);
	    	string[] parts = message.Split(',');

	    	Vector3 vec = Vector3.zero;
	    	if(parts.Length > 1){
	    		vec = new Vector3(float.Parse(parts[0]), float.Parse(parts[1]), float.Parse(parts[2]));
	    	}
	    	if (header == "[o]"){
	    		vec = new Vector3(-vec.y, vec.x, vec.z);
	    		rov.eulerAngles = vec;
	    	} else if (header == "[g]"){
	    		vec = new Vector3(vec.x / 640, vec.y / 640, vec.z / 640);
	    		rov.position = vec;
	    	} else if (header == "[t]"){
	    		temp = int.Parse(message);
	    	}
    	}
    }

    public string readData(int timeout){
    	stream.ReadTimeout = timeout;
    	try {
    		return stream.ReadLine();
    	}
    	catch (TimeoutException e) {
    		Debug.Log(e);
    		return null;
    	}
    }
}
