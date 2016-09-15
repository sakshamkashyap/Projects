#pragma strict

var radius : float = 5.0; //provides a radius which explosion will effect
var power : float = 10.0; // provides explosive power
var explosiveLift : float = 1.0; //Higher value means rigidbody will fly upward
var explosiveDelay : float = 5.0; //adds delay in seconds to explosion

function Start () {

    yield WaitForSeconds(explosiveDelay);
    var grenadeOrigin : Vector3 = transform.position;
    var colliders : Collider[] = Physics.OverlapSphere (grenadeOrigin, radius);

    for(var hit : Collider in colliders){

        if (hit.GetComponent.<Rigidbody>()){

        hit.GetComponent.<Rigidbody>().AddExplosionForce(power, grenadeOrigin, radius, explosiveLift);
        Destroy(gameObject);
    }
    }
}

function Update () {

}