using UnityEngine;
using System.Collections;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class GamePlay : MonoBehaviour {

	public static bool isPaused = false;
	public Text pauseText;
	public GameObject[] weapons;
	public GameObject zombie;

	void Start()
	{
		weapons [PlayerPrefs.GetInt ("weapon")].SetActive (true);
	}

	public void RestartGame()
	{
		if (isPaused)
		{
			Time.timeScale = 1;
			isPaused = false;
		}
		PlayerPrefs.SetInt ("weapon", PlayerPrefs.GetInt ("weapon"));
		SceneManager.LoadScene (SceneManager.GetActiveScene().name);
		weapons [PlayerPrefs.GetInt ("weapon")].SetActive (true);
	}

	public void PauseGame()
	{
		if (!isPaused) {
			Time.timeScale = 0f;
			isPaused = true;
			pauseText.text = "Play";
		} else {
			Time.timeScale = 1f;
			isPaused = false;
			pauseText.text = "Pause";
		}
	}

	public void SwitchWeapon()
	{
		if(weapons[0].activeSelf)
		{
			weapons[0].SetActive(false);
			weapons[1].SetActive(true);
			PlayerPrefs.SetInt ("weapon", 1);
		}
		else if(weapons[1].activeSelf)
		{
			weapons[1].SetActive(false);
			weapons[2].SetActive(true);
			PlayerPrefs.SetInt ("weapon", 2);
		}
		else if(weapons[2].activeSelf)
		{
			weapons[2].SetActive(false);
			weapons[0].SetActive(true);
			PlayerPrefs.SetInt ("weapon", 0);
		}
	}
}
