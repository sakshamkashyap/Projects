using UnityEngine;
using System.Collections;

public class NavMeshZombieFollow : MonoBehaviour {

	NavMeshAgent agent;
	AudioSource zombieAudioSource;
	public AudioClip attackAudio;
	bool oneTime = false;

	void Start () {
		agent = GetComponent<NavMeshAgent> ();
		zombieAudioSource = GetComponent<AudioSource> ();
	}


	void Update () {
		
		agent.SetDestination (GameObject.FindGameObjectWithTag ("Player").transform.position);
		if (agent.remainingDistance <= agent.stoppingDistance && agent.remainingDistance != 0) {
			GetComponent<Animator>().SetBool ("ZombieAttack", true);
			if (!oneTime) {
				zombieAudioSource.clip = attackAudio;
				zombieAudioSource.Play ();
				oneTime = true;
			}
		} else {
			GetComponent<Animator>().SetBool ("ZombieAttack", false);
			oneTime = false;
			zombieAudioSource.Stop ();
		}
	}
		
}
