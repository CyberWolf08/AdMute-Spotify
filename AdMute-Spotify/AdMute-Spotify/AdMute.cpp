#include "admute.h"
#include <stdio.h>

void MuteAd();

#define RefreshTime 5000

int main(int argc, char **argv) 
{
	wchar_t AdList[20][256], *CurrentTrack;
	HWND SpotifyHandle;
	int isMuted = 0, AdCount;
	
	// load the settings
	AdCount = InitSettings(AdList);

	CurrentTrack = (wchar_t *)malloc(256 * sizeof(wchar_t));

	// begin a infinite loop. This will be dealt with later
	while(1)
	{
		// at each iteration check if spotify is running
		SpotifyHandle = GetSpotifyHandle();
		if(SpotifyHandle == NULL)
		{
			printf("Spotify is closed. Retrying to find spotify in 5 seconds.");
			Sleep(RefreshTime);
			continue;
		}
		wcscpy(CurrentTrack, GetNowPlaying(SpotifyHandle));
		// if track is an ad and volume is not muted, then mute the ad
		if(TrackIsAd(CurrentTrack, AdList, AdCount) && !isMuted)
		{
			MuteAd();
			isMuted = 1;
			printf("Track \"%ls\" is a ad, we will mute it", CurrentTrack);
			continue;
		}
		// if track is not an ad and volume is muted, unmute
		if(!TrackIsAd(CurrentTrack, AdList, AdCount) && isMuted)
		{
			printf("Track \"%ls\" is a not ad, we will unmute it", CurrentTrack);
			MasterVolumeMuteUnmute();
			isMuted = 0;
		}
	}

	getchar();
	return 0;
}

int InitSettings(wchar_t AdList[20][256])
{
	FILE *SettingsFile;
	//try to open the settings file
	SettingsFile = fopen("settings.ini", "r");
	
	if(SettingsFile == NULL)
	{
		puts("No settings file found. Carrying on with no AdList");
		return;
	}

	// there are no more than 20 ads usually 
	wchar_t aux;
	int i = 0, j = 0;
	while (aux = fgetwc(SettingsFile) != EOF)
	{
		if(aux == L'\n') {
			// terminate the string
			AdList[i][j] = L'0';
			i++; j = 0;
		}
		AdList[i][j] = aux;
		j++;
	}
	return i;
}

int TrackIsAd(const wchar_t *Track, wchar_t AdList[20][256], int AdCount) //0 false, 1 true
{
	wchar_t *aux;

	for(int i = 0;i < AdCount; i++) {
		aux = &AdList[i][0];
		if(!wcscmp(Track, aux))
			return 1;
	}
	return 0;
}

void MuteAd()
{
	MasterVolumeMuteUnmute();
	Sleep(500);
	PlayPause(GetSpotifyHandle());
}