# Fabrique les 3 bruitages de Pong (WAV mono 16 bits, 18 500 Hz : le format ADPCM-A)
import sys, wave, struct, math
out = sys.argv[1]
RATE = 18500
def ecrire(nom, echantillons):
    w = wave.open(f"{out}/{nom}.wav", "wb")
    w.setnchannels(1); w.setsampwidth(2); w.setframerate(RATE)
    w.writeframes(b"".join(struct.pack("<h", max(-32767, min(32767, int(v)))) for v in echantillons))
    w.close()
def carre(freq_debut, freq_fin, duree, volume, decroissance):
    n = int(RATE * duree); phase = 0.0; res = []
    for i in range(n):
        t = i / n
        f = freq_debut + (freq_fin - freq_debut) * t
        phase += f / RATE
        v = 1.0 if (phase % 1.0) < 0.5 else -1.0
        env = (1 - t) ** decroissance
        res.append(v * env * volume * 32767)
    return res
ecrire("raquette", carre(880, 880, 0.06, 0.55, 1.0))   # bip court et clair
ecrire("mur", carre(330, 330, 0.05, 0.45, 1.5))        # plus grave, plus sourd
ecrire("point", carre(660, 165, 0.45, 0.55, 0.7))      # descend : point perdu
