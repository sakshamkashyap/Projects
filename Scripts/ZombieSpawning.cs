using UnityEngine;
using System.Collections;

public class ZombieSpawning : MonoBehaviour {

	public GameObject[] spawnPoints;
	public GameObject enemy;
	// Use this for initialization
	void Start () {
		spawnPoints = GameObject.FindGameObjectsWithTag ("SpawnP");
	}
	
	// Update is called once per frame
	void Update () {
		if (!GamePlay.isPaused) {
			GameObject[] enemies;
			enemies = GameObject.FindGameObjectsWithTag ("Enemy");
			if (enemies.Length >= 3)
				;
			else
				InvokeRepeating ("spawnZombies", 1, 0.5f);
		}
		
	}

	void spawnZombies()
	{
		int spawnPos = Random.Range (0, (spawnPoints.Length - 0));
		Instantiate (enemy, spawnPoints[0].transform.position,  Quaternion.Euler(0, -90, 0));
		Instantiate (enemy, spawnPoints[1].transform.position,  Quaternion.Euler(0, -90, 0));
		Instantiate (enemy, spawnPoints[2].transform.position,  Quaternion.Euler(0, -90, 0));
		CancelInvoke ();
	}
}
