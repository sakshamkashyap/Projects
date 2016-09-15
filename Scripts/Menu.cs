using UnityEngine;
using System.Collections;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class Menu : MonoBehaviour {
	
	public GameObject playGameText;
	public GameObject quitGameText;
	public GameObject theText;
	public AudioClip selectedOptionSound;
	public AudioClip selectSound;
	public Canvas blackScreen;
	public Image myImage;

	public void loadScene()
	{
		blackScreen.gameObject.SetActive (true);
		blackScreen.gameObject.GetComponent<AudioSource> ().clip = selectedOptionSound;
		blackScreen.gameObject.GetComponent<AudioSource>().Play ();
		playGameText.gameObject.GetComponent<TextMesh> ().text = ""; 
		quitGameText.gameObject.GetComponent<TextMesh> ().text = ""; 
		StartCoroutine (FadeToClear());
	}

	public void changeTextToRed()
	{
		GetComponent<AudioSource> ().clip = selectSound;
		GetComponent<AudioSource>().Play ();
		theText.GetComponent<TextMesh>().color = Color.red; 
	}

	public void changeTextToWhite()
	{
		GetComponent<AudioSource> ().clip = selectSound;
		GetComponent<AudioSource>().Play ();
		theText.GetComponent<TextMesh>().color  = Color.white; 
	}

	public void quitGame()
	{
		GetComponent<AudioSource> ().clip = selectedOptionSound;
		GetComponent<AudioSource>().Play ();
		Application.Quit ();
	}
		

	private IEnumerator FadeToClear()
	{
		float fadeTime = 2.0f ;
		float rate = 1.0f / fadeTime;
		float progress = 0.0f;
		while(progress < 0.4f)
		{
			myImage.color = Color.Lerp (myImage.color, Color.clear, progress);
			if (myImage.color.a >= 0.99999)
				break;
			progress += rate * Time.deltaTime;
			yield return null;
		}
		SceneManager.LoadScene ("WeaponMenu");
	}
		
}
