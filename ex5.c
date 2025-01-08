/******************
Name: Yotam Yakov
ID: 207918822
Assignment: ex5
*******************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Song {
    char* title;
    char* artist;
    int year;
    char* lyrics;
    int streams;
    struct Song* next;
} Song;

typedef struct Playlist {
    char* name;
    Song** songs;
    int songsNum;
    struct Playlist* next;
} Playlist;

enum Sorter {
    YEAR = 0,
    STREAMS_ASC = 1,
    STREAMS_DES = 2,
    ALPHABET = 3
};

Playlist* playlistHead = NULL;
Playlist* playlistLast = NULL; 

// PRINTING FUNCTIONS:
// Print main menu
void printMainMenu() {
    printf("Please Choose:\n\
        1. Watch playlists\n\
        2. Add playlist\n\
        3. Remove playlist\n\
        4. exit\n");
    
    return;
}

// Print playlist menu
void printPlaylistMenu() {
    printf("\
    1. Show Playlist\n\
    2. Add Song\n\
    3. Delete Song\n\
    4. Sort\n\
    5. Play\n\
    6. exit\n");
        
    return;
}

// Print sort menu
void printSortMenu() {
    printf("choose:\n");
    printf("1. sort by year\n");
    printf("2. sort by streams - ascending order\n");
    printf("3. sort by streams - descending order\n");
    printf("4. sort alphabetically\n");
    
    return;
}

// Print playlists return last index
int printPlaylists() {
    Playlist* head = playlistHead;
    int index = 1;

    printf("Choose a playlist:\n");
    // While there is a playlist print it
    while(head) {
        printf("%d. %s\n", index, head->name);
        index++;
        head = head->next;
    }
    printf("%d. Back to main menu\n", index);

    // Return last index for going back
    return index;
}

// Print songs
void printSongs(Playlist* playlist) {
    // Run only if there are songs
    if(playlist->songs){
        // Set head
        Song* head = playlist->songs[0];
    
        // Print data for each song
        for(int i = 1; i <= playlist->songsNum; i++) {
            char* title = head->title;
            char* artist = head->artist;
            int year = head->year;
            int streams = head->streams;

            printf("%d. Title: %s\n\
        Artist: %s\n\
        Released: %d\n\
        Streams: %d\n\n", i, title, artist, year, streams);
            head = head->next;
        }
    }
    
    return;
}

// ERROR HANDLING:
// Handle invalid option
void invalidOption() {
    printf("Invalid option\n");
    
    return;
}

// Handle failed allocation
void failedAlloc() {
    printf("Memory allocation failed!\n");
    
    exit(1);
}

// GET BY INDEX:
// Find playlist by its index
Playlist* getPlaylistByIndex(int index) {
    Playlist* head = playlistHead;
    int playlist = 1;

    while(playlist < index) {
        // If index was not found return 0
        if(!head) {
            return 0;
        }
        // If index found return playlist
        if(playlist == index) {
            return head;
        }
        // Iterate through the list
        head = head->next;
        playlist++;
    }
    return head;
}

// Find song by its index
Song* getSongByIndex(Playlist* playlist, int index) {
    Song** songs = playlist->songs;
    
    // Validate list not empty
    if(songs) {
        Song* head = *songs;
        int song = 1;

        while(song <= index) {
            // If index was not found return 0
            if(!head) {
                return 0;
            }
            // If index found return song
            if(song == index) {
                return head;
            }
            // Iterate through the list
            head = head->next;
            song++;
        }
    }
    return 0;
}

// ADDING FUNCTIONS:
// Scan chars to create malloced string
char* scanString() {
    char c = 0;
    int size = 1, index = 0;
    char* name = (char *)malloc(size * sizeof(char));
    
    // Handle failed malloc
    if(!name) {
        failedAlloc();
    }

    // For each char realloc and add char
    while(scanf("%c", &c) == 1 && c != '\n') {
        name[index] = c;
        index++;
        size++;
        // Resize for new char
        name = realloc(name, size * sizeof(char));
        // Handle failed malloc
        if(!name) {
            failedAlloc();
        }
    }

    // Return pointer to string head
    return name;
}

// Scan input for full song
Song* scanSongDetails() {
    Song* song = malloc(sizeof(Song));

    printf("Enter song's details\n");
    // Scan title
    printf("Title:\n");
    // Clear \n
    scanf("%*c");
    song->title = scanString();
    // Scan artist
    printf("Artist:\n");
    song->artist = scanString();
    // Scan year
    printf("Year of release:\n");
    scanf("%d", &song->year);
    // Scan lyrics
    printf("Lyrics:\n");
    // Clear \n
    scanf("%*c");
    song->lyrics = scanString();
    song->next = NULL;

    return song;
}

// Add a new playlist
void addPlaylist() {
    Playlist* playlist = (Playlist*) malloc(sizeof(Playlist));
    
    // Scan playlist name
    printf("Enter playlist's name:\n");
    scanf("%*c");
    playlist->name = scanString();
    playlist->songs = NULL;
    playlist->songsNum = 0;
    
    // If first set as playlistHead
    if(!playlistHead) {
        playlistHead = playlist;
    }
    playlist->next = NULL;
    // If list not empty point last to new playlist
    if(playlistLast) {
        playlistLast->next = playlist;
    }
    // Set as new playlistLast
    playlistLast = playlist;
    
    return;
}

// Ass a new song to playlist
void addSongToPlaylist(Playlist* playlist) {
    Song* song = scanSongDetails();
    if(!playlist->songs) {
        playlist->songs = (Song**) malloc(sizeof(Song*));
    } else {
        playlist->songs = (Song**) realloc(playlist->songs, (playlist->songsNum + 1) * sizeof(Song*));
    }
    
    // Handle failed allocation
    if(!playlist->songs) {
        failedAlloc();
    }

    // Validate list not empty
    if(playlist->songs[0]) {
        Song* head = playlist->songs[0];
        // Get to current last song
        while(head->next) {
            head = head->next;
        }
        // Point last to new
        head->next = song;
        // If list empty set as first song
    } else {
        playlist->songs[0] = song;
    }
    // Add to songs counter
    playlist->songsNum++;
    
    return;
}

// REMOVING FUNCTIONS:
// Free memory allocated for this song
void freeSongItem(Song* song) {
    if(song != NULL) {
        free(song->title);
        free(song->artist);
        free(song->lyrics);
        freeSongItem(song->next);
        free(song);
    }
    
    return;
}

// Delete song
void deleteSong(Playlist* playlist) {
    printSongs(playlist);

    // Scan for requested song
    int index;
    printf("choose a song to delete, or 0 to quit:\n");
    scanf("%d", &index);

    // Find song
    Song* song = getSongByIndex(playlist, index);

    // If song is first rewire list head
    if(index == 1) {
        playlist->songs[0] = song->next;
    // If not first rewire previous song
    } else if (index > 1 && index <= playlist->songsNum) {
        Song* prevSong = getSongByIndex(playlist, index - 1);
        prevSong->next = song->next;
        song->next = NULL;
        freeSongItem(song);
        playlist->songsNum--;
        printf("Song deleted successfully.\n");
    }
    
    return;
}

// Free memory allocated for this playlist
void freePlaylist(Playlist* playlist) {
    if(playlist->next) {
        freePlaylist(playlist->next);
    }
    freeSongItem(playlist->songs[0]);
    free(playlist->songs);
    free(playlist->name);
    free(playlist);
    
    return;
}

// Delete playlist
void deletePlaylist() {
    int back = printPlaylists();
    int index;

    // Scan for requested playlist
    scanf("%d", &index);

    // Find playlist
    Playlist* playlist = getPlaylistByIndex(index);

    if(index == back) {
        return;
    }

    // If playlist is first rewire playlistHead
    if(index == 1) {
        playlistHead = playlist->next;
    //If not first rewire previous playlist
    } else if (index > 1 && index < back) {
        Playlist* prevPlaylist = getPlaylistByIndex(index - 1);
        prevPlaylist->next = playlist->next;
    }
    // Free memory
    playlist->next = NULL;
    freePlaylist(playlist);
    printf("Playlist deleted.\n");
    
    return;
}

// SORTING FUNCTIONS:
// Swap data between 2 songs
void swapSongs(Song* songA, Song* songB) {
    // Save temp data
    char* title = songA->title;
    char* artist = songA->artist;
    int year = songA->year;
    char* lyrics = songA->lyrics;
    int streams = songA->streams;

    // Move B to A
    songA->title = songB->title;
    songA->artist = songB->artist;
    songA->year = songB->year;
    songA->lyrics = songB->lyrics;
    songA->streams = songB->streams;

    // Move temp to B
    songB->title = title;
    songB->artist = artist;
    songB->year = year;
    songB->lyrics = lyrics;
    songB->streams = streams;
    
    return;
}

// Get the relevent sorting type
int comparer(Song* song1, Song* song2, enum Sorter sort) {
    switch(sort) {
    case YEAR:
        return song1->year - song2->year;
    case STREAMS_ASC:
        return song1->streams - song2->streams;
    case STREAMS_DES:
        return song2->streams - song1->streams;
    case ALPHABET:
        return strcmp(song1->title, song2->title);
    }

    return 0;
}

// Bubble sort songs by sorting type
void sortSongs(Song** songs, enum Sorter sort) {
    Song* song;

    // Optimization
    int swap = 1;
    while(swap) {
        swap = 0;
        // Get current song
        song = *songs;
        while(song->next != NULL) {
            // If unordered swap songs
            if(comparer(song, song->next, sort) > 0) {
                swap = 1;
                swapSongs(song, song->next);
            }
            // Iterate to next song
            song = song->next;
        }
    }
    
    return;
}

// Direct correct sorting type by given input
void sortPlaylist(Playlist* playlist) {
    int choice;
    printSortMenu();

    // Scan for desired sorting type
    scanf("%d", &choice);
    // Direct base on input
    switch(choice) {
    case 1:
        sortSongs(playlist->songs, YEAR);
        break;
    case 2:
        sortSongs(playlist->songs, STREAMS_ASC);
        break;
    case 3:
        sortSongs(playlist->songs, STREAMS_DES);
        break;
    default:
        sortSongs(playlist->songs, ALPHABET);
        break;
    }
    printf("sorted\n");
    
    return;
}

// PLAY FUNCTIONS:
// Play a single song
void playSong(Playlist* playlist, int index) {
    Song* song = getSongByIndex(playlist, index);
    printf("Now playing %s:\n", song->title);
    printf("$ %s $\n", song->lyrics);
    song->streams++;
    
    return;
}

// Play all songs from playlist
void playAllSongs(Playlist* playlist) {
    // Iterate all songs and play
    for(int i = 1; i <= playlist->songsNum; i++) {
        playSong(playlist, i);
        printf("\n");
    }
}

// PLAYLIST FUNCTIONS:
// Show all songs from given playlist
void showPlaylistSong(Playlist* playlist) {
    printSongs(playlist);

    // Scan for requested song
    int choice;
    printf("choose a song to play, or 0 to quit:\n");
    scanf("%d", &choice);

    // While is a valid song play it
    while(choice > 0 && choice <= playlist->songsNum) {
        playSong(playlist, choice);
        printf("choose a song to play, or 0 to quit:\n");
        scanf("%d", &choice);
    }

    return;
}

// Show all available playlists
void showAllPlaylists() {
    int choice, back = printPlaylists();
    Playlist* playlist;
    
    // Scan for requested playlist
    scanf("%d", &choice);
    while(choice > back) {
        invalidOption();
        scanf("%d", &choice);
    }

    // While valid playlist go into playlist menu
    while(choice < back && choice > 0) {
        // Get requested playlist by index
        playlist = getPlaylistByIndex(choice);
        // Print menu and scan for input
        printf("playlist %s:\n", playlist->name);
        printPlaylistMenu();
        scanf("%d", &choice);
        //Direct correct function for user input
        while(choice != 6) {
            switch(choice) {
            case 1:
                showPlaylistSong(playlist);
                break;
            case 2:
                addSongToPlaylist(playlist);
                break;
            case 3:
                deleteSong(playlist);
                break;
            case 4:
                sortPlaylist(playlist);
                break;
            case 5:
                playAllSongs(playlist);
                break;
            default:
                break;
            }
            printPlaylistMenu(playlist->name);
            scanf("%d", &choice);
        }
        printPlaylists();
        scanf("%d", &choice);
    }

    return;
}

// MAIN FUNCTION
int main() {
    int choice = -1;
    
    do {
        printMainMenu();
        if(scanf("%d", &choice)) {
            switch(choice) {
            case 1:
                showAllPlaylists();
                break;
            case 2:
                addPlaylist();
                break;
            case 3:
                deletePlaylist();
                break;
            case 4:
                break;
            default:
                invalidOption();
                break;
            }
        }
    } while (choice != 4);
    
    // Free all on exit
    if(playlistHead) {
        freePlaylist(playlistHead);
    }
    
    printf("Goodbye!\n");  
}
