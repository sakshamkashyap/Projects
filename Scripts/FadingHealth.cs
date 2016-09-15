using UnityEngine;
using System.Collections;

public class FadingHealth : MonoBehaviour {

	Texture tex1;
	Texture tex2;
	Texture tex3;
	Texture tex4;
	Texture tex5;

	void Start()
	{
		tex1 = Resources.Load("blood_screen1") as Texture;
		tex2 = Resources.Load("blood_screen2")as Texture;
		tex3 = Resources.Load("blood_screen3")as Texture;
		tex4 = Resources.Load("blood_screen4")as Texture;
		tex5 = Resources.Load("blood_screen5")as Texture;
	}

	double health = 100.0;
	double maxHealth = 100.0;
	bool enterCollider = false;


	void OnTriggerEnter(Collider col)
	{
	 if(col.gameObject.tag == "KD")
	 {
	  enterCollider = true;
	 }
	}

	void OnTriggerExit(Collider col)
	{
	 if(col.gameObject.tag == "KD")
	 {
	  enterCollider = false;
	 }
	}

	void Update()
	{
	 if (health > 0 && enterCollider == true)
	 {
	  health -= 2.5;
	 }
	 if (health >= 0 && enterCollider == false)
	 {
	  health += 0.05;
	 }
	 if (health > maxHealth)
	 {
	  health = maxHealth;
	 }
	}

	void OnGUI()
	{
		if (health <= 90)
		{
			GUI.DrawTexture(new Rect(0,0, Screen.width, Screen.height),tex1);
		}
		if (health <= 85)
		{
			GUI.DrawTexture(new Rect(0,0, Screen.width, Screen.height),tex2);
		}
		if (health <= 80)
		{
			GUI.DrawTexture(new Rect(0,0, Screen.width, Screen.height),tex3);
		}
		if (health <= 75)
		{
			GUI.DrawTexture(new Rect(0,0, Screen.width, Screen.height),tex4);
		}
		if (health <= 70)
		{
			GUI.DrawTexture(new Rect(0,0, Screen.width, Screen.height),tex5);
		}
	}
}
