# Genere assets/pong-sprites.gif (tuiles 16x16, ligne par ligne, couleur 0 = transparente)
import sys
from PIL import Image
level = sys.argv[1]   # 'base' (pong-2), 'chiffres' (pong-8), 'anim' (bonus balle)
out = sys.argv[2]
rows = {'base': 1, 'chiffres': 4, 'anim': 5}[level]
im = Image.new('P', (64, 16 * rows), 0)
pal = [0,0,0, 255,255,255, 255,224,64, 96,224,255, 255,128,64] + [0,0,0]*11
im.putpalette(pal)
px = im.load()
def rect(x0, y0, w, h, c=1):
    for y in range(y0, y0 + h):
        for x in range(x0, x0 + w):
            px[x, y] = c
# tuile 256 (x 0-15) + 257 (x 16-31) : raquette 5 x 20, coupee en 16 + 4 lignes
rect(0, 0, 5, 16)
rect(16, 0, 5, 4)
# tuile 258 (x 32-47) : balle 4 x 4
rect(32, 0, 4, 4)
DIGITS = {
 '0': ["111","101","101","101","111"], '1': ["010","110","010","010","111"],
 '2': ["111","001","111","100","111"], '3': ["111","001","111","001","111"],
 '4': ["101","101","111","001","001"], '5': ["111","100","111","001","111"],
 '6': ["111","100","111","101","111"], '7': ["111","001","001","001","001"],
 '8': ["111","101","111","101","111"], '9': ["111","101","111","001","111"],
}
if rows >= 4:
    # tuiles 260 a 269 : chiffres 0 a 9 (cellules de 3 x 3 pixels -> 9 x 15)
    for d in range(10):
        t = 4 + d                      # tuile 256 + 4 + d
        tx, ty = (t % 4) * 16, (t // 4) * 16
        for r, row in enumerate(DIGITS[str(d)]):
            for c, v in enumerate(row):
                if v == '1':
                    rect(tx + 3 + c * 3, ty + r * 3, 3, 3)
if rows >= 5:
    # tuiles 272 a 275 : balle animee (4 images, couleurs 1 a 4)
    for f, col in enumerate([1, 2, 1, 3]):
        rect(f * 16, 64, 4, 4, col)
im.save(out)
print(out, im.size)
