using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RotateCamera : MonoBehaviour
{
    public int cameraMode;
    public Transform rov;
    // Start is called before the first frame update
    void Start()
    {
        cameraMode = 0;
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKey(KeyCode.A)){
        	transform.RotateAround(new Vector3(0,0,0), Vector3.up, 80 * Time.deltaTime);
        }
        if (Input.GetKey(KeyCode.D)){
        	transform.RotateAround(new Vector3(0,0,0), Vector3.up, -80 * Time.deltaTime);
        }
        if (Input.GetKeyDown(KeyCode.F)){
            if (cameraMode == 0){
                cameraMode = 1;
                transform.parent = null;
                transform.position = new Vector3 (0, 2, -4);
                transform.eulerAngles = new Vector3 (20, 0, 0);
            } else {
                cameraMode = 0;
                transform.position = new Vector3 (0, 2, -4);
                transform.eulerAngles = new Vector3 (20, 0, 0);
                rov.eulerAngles = new Vector3 (0, 0, 0);
                rov.position = new Vector3 (0, 0, 0);
                transform.parent = rov;
            }
        }
    }
}
