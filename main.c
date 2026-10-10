/// This file is a template to boot your first ROM

#include <ngdevkit/neogeo.h>
#include <ngdevkit/bios-calls.h>
#include <ngdevkit/ng-fix.h>


int main(void) {
  bios_fix_clear();

  // Set up a minimal palette
  const u16 palette[] = {0x8000, 0x0fff, 0x0555};
  for (u16 i=0; i<3; i++) {
      MMAP_PALBANK1[i] = palette[i];
  }

  // Template code
  ng_center_text(15, 0, "NGDEVKIT TEMPLATE ROM");

  // ngdevkit's default VBlank handler rearms the watchdog for you
  for(;;) {}
  return 0;
}
