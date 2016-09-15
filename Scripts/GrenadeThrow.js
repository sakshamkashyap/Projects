#pragma strict

var grenade : Rigidbody; //our Prefab
var throwPower : float =10 ; //how fast object will throw

function Start () {

}

function Update () {

    if (Input.GetButtonDown("Fire1")){
        var clone : Rigidbody; 
        clone = Instantiate(grenade, transform.position, transform.rotation);
        clone.velocity = transform.TransformDirection(Vector3.forward * throwPower);
    }
}