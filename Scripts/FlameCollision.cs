using UnityEngine;
using System.Collections;

public class FlameCollision : MonoBehaviour {

	ParticleSystem explosion;
	ParticleSystem fire;
	AudioSource zombieAudioSource;
	public AudioClip hurtAudio;
	public AudioClip explosionAudio;


	void OnParticleCollision(GameObject other) {

		if (other.CompareTag ("Fire")) {
			ParticleSystem[] ps = gameObject.transform.parent.GetComponentsInChildren<ParticleSystem>();
			foreach(ParticleSystem p in ps)
			{
				if (p.name == "Fire")
					fire = p;
				else if (p.name == "Destruction01")
					explosion = p;	
			}
			zombieAudioSource = GetComponent<AudioSource> ();
		    zombieAudioSource.clip = hurtAudio;
			zombieAudioSource.Play ();
			fire.Play ();
			Invoke ("explode", 3.2f);
		}
	}

	void explode()
	{
		zombieAudioSource.Stop ();
		zombieAudioSource.clip = explosionAudio;
		zombieAudioSource.Play ();
		explosion.Play();
		Destroy (gameObject.transform.parent.gameObject,1.5f);
	}
}
