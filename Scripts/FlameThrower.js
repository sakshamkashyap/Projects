#pragma strict

var flameThrower : ParticleSystem;

function Start()
{
 //flameThrower.emissionRate = 0.0f; 
// flameThrower.Play();
// GameObject.Find("FlameCollider").GetComponent(Collider).enabled = false ;
}

function Update()
{
 if(Input.GetMouseButton(0))
 {
   //flameThrower.emissionRate = 50.0f; 
   GetComponent.<AudioSource>().Play ();
   flameThrower.Play();
 //GameObject.Find("FlameCollider").GetComponent(Collider).enabled = true ;
 }
 else
 {
 //flameThrower.emissionRate = 0.0f; 
 flameThrower.Stop();
 //GameObject.Find("FlameCollider").GetComponent(Collider).enabled = false ;
 }
}

 