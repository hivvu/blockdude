// audio.c
#include <gb/gb.h>
#include <gb/hardware.h>
#include <stdint.h>

// Define notes
// Note definitions for Game Boy (based on frequency)
#define NOTE_C3  44
#define NOTE_D3  49
#define NOTE_E3  52
#define NOTE_F3  56
#define NOTE_G3  59
#define NOTE_A3  61
#define NOTE_B3  63
#define NOTE_C4  65
#define NOTE_D4  69
#define NOTE_E4  73
#define NOTE_F4  77
#define NOTE_G4  79
#define NOTE_A4  81
#define NOTE_B4  83
#define NOTE_C5  87
#define NOTE_D5  92
#define NOTE_E5  95
#define NOTE_F5  98
#define NOTE_G5  101
#define NOTE_A5  103
#define NOTE_B5  105
#define NOTE_C6  113
#define NOTE_REST 0
#define NOTE_NOISE(x) (0x80 | (x))

// Melody for Pulse 1 channel
const unsigned char melody[] = {
    NOTE_C5, NOTE_E5, NOTE_G5, NOTE_C6, NOTE_G5, NOTE_E5, NOTE_C5, NOTE_REST,
    NOTE_A4, NOTE_C5, NOTE_E5, NOTE_A5, NOTE_E5, NOTE_C5, NOTE_A4, NOTE_REST,
    NOTE_F4, NOTE_A4, NOTE_C5, NOTE_F5, NOTE_C5, NOTE_A4, NOTE_F4, NOTE_REST,
    NOTE_G4, NOTE_B4, NOTE_D5, NOTE_G5, NOTE_D5, NOTE_B4, NOTE_G4, NOTE_REST,
    0xFF // End of melody
};

// Bassline for Pulse 2 channel
const unsigned char bassline[] = {
    NOTE_C3, NOTE_REST, NOTE_G3, NOTE_REST, NOTE_E3, NOTE_REST, NOTE_G3, NOTE_REST,
    NOTE_A3, NOTE_REST, NOTE_E3, NOTE_REST, NOTE_C3, NOTE_REST, NOTE_E3, NOTE_REST,
    NOTE_F3, NOTE_REST, NOTE_C3, NOTE_REST, NOTE_A3, NOTE_REST, NOTE_C3, NOTE_REST,
    NOTE_G3, NOTE_REST, NOTE_D3, NOTE_REST, NOTE_B3, NOTE_REST, NOTE_D3, NOTE_REST,
    0xFF // End of bassline
};

// Arpeggio for Wave channel
const unsigned char arpeggio[] = {
    NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5, NOTE_G4, NOTE_E4, NOTE_C4, NOTE_REST,
    NOTE_A3, NOTE_C4, NOTE_E4, NOTE_A4, NOTE_E4, NOTE_C4, NOTE_A3, NOTE_REST,
    NOTE_F3, NOTE_A3, NOTE_C4, NOTE_F4, NOTE_C4, NOTE_A3, NOTE_F3, NOTE_REST,
    NOTE_G3, NOTE_B3, NOTE_D4, NOTE_G4, NOTE_D4, NOTE_B3, NOTE_G3, NOTE_REST,
    0xFF // End of arpeggio
};

// Drum pattern for Noise channel
const unsigned char drums[] = {
    NOTE_NOISE(0x1F), NOTE_REST, NOTE_NOISE(0x1F), NOTE_REST,
    NOTE_NOISE(0x1F), NOTE_REST, NOTE_NOISE(0x1F), NOTE_REST,
    NOTE_NOISE(0x1F), NOTE_REST, NOTE_NOISE(0x1F), NOTE_REST,
    NOTE_NOISE(0x1F), NOTE_REST, NOTE_NOISE(0x1F), NOTE_REST,
    0xFF // End of drums
};

// Function to play a note on a specific channel
void play_note(uint8_t channel, uint8_t note, uint8_t length) {
    if (note == NOTE_REST) {
        NR51_REG &= ~(1 << channel); // Mute channel
    } else {
        NR51_REG |= (1 << channel);  // Unmute channel
        switch (channel) {
            case 0: NR10_REG = 0x00; NR11_REG = 0x80; NR12_REG = 0x73; NR13_REG = note; NR14_REG = 0x86; break; // Pulse 1
            case 1: NR21_REG = 0x80; NR22_REG = 0x73; NR23_REG = note; NR24_REG = 0x86; break; // Pulse 2
            case 2: NR30_REG = 0x80; NR31_REG = 0x00; NR32_REG = 0x73; NR33_REG = note; NR34_REG = 0x86; break; // Wave
            case 3: NR41_REG = 0x00; NR42_REG = 0x73; NR43_REG = note; NR44_REG = 0x86; break; // Noise
        }
    }
    delay(length);
}

// Main music loop
void play_music() {
    uint8_t i = 0;
    while (1) {
        if (melody[i] == 0xFF) i = 0; // Loop melody
        play_note(0, melody[i], 100); // Pulse 1
        play_note(1, bassline[i], 100); // Pulse 2
        play_note(2, arpeggio[i], 100); // Wave
        play_note(3, drums[i], 100); // Noise
        i++;
    }
}