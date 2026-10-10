;;; Pong pour Neo Geo - le pilote son (processeur Z80).
;;; Le 68000 ecrit un numero de commande dans REG_SOUND (0x320000) ;
;;; le pilote nullsound de ngdevkit saute a la ligne correspondante
;;; de cette table, et nous jouons l'echantillon ADPCM-A demande.

        .include "helpers.inc"

        .area   CODE

cmd_jmptable::
        ;; commandes 0 a 3 : reservees par le BIOS et ngdevkit
        jp      snd_command_unused
        jp      snd_command_01_prepare_for_rom_switch
        jp      music_ngdevkit_eye_catcher
        jp      snd_command_03_reset_driver
        ;; nos commandes : 4, 5 et 6
        jp      son_raquette
        jp      son_mur
        jp      son_point
        init_unused_cmd_jmptable

son_raquette:
        ld      ix, #adpcm_a_raquette
        call    snd_adpcm_a_play
        ret

son_mur:
        ld      ix, #adpcm_a_mur
        call    snd_adpcm_a_play
        ret

son_point:
        ld      ix, #adpcm_a_point
        call    snd_adpcm_a_play
        ret

        ;; position des echantillons dans la ROM V (fichier genere)
        .include "assets/samples.inc"

        ;; un echantillon = debut, fin, canal, volume
adpcm_a_raquette:
        .db     RAQUETTE_START_LSB
        .db     RAQUETTE_START_MSB
        .db     RAQUETTE_STOP_LSB
        .db     RAQUETTE_STOP_MSB
        .db     0                       ; canal 1
        .db     0xdf                    ; gauche + droite, volume maximal
        .db     1                       ; canal 1 (bit)

adpcm_a_mur:
        .db     MUR_START_LSB
        .db     MUR_START_MSB
        .db     MUR_STOP_LSB
        .db     MUR_STOP_MSB
        .db     1                       ; canal 2
        .db     0xdf
        .db     2                       ; canal 2 (bit)

adpcm_a_point:
        .db     POINT_START_LSB
        .db     POINT_START_MSB
        .db     POINT_STOP_LSB
        .db     POINT_STOP_MSB
        .db     2                       ; canal 3
        .db     0xdf
        .db     4                       ; canal 3 (bit)
