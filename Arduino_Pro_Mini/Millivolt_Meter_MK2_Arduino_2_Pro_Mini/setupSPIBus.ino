void setupSPIBus(void) {
  SPI.begin();                           //initialise SPI bus
  SPI.setBitOrder(MSBFIRST);             //Sets the order of bits shifted out and in to SPI bus, MSBFIRST (most-significant bit first)
  SPI.setDataMode(SPI_MODE0);            // Mode 0 (MOSI read on rising edge (CPLI=0) and SCK idle low (CPOL=0))
  SPI.setClockDivider(SPI_CLOCK_DIV16);  // Divide Arduino clock by 16 to give a 1 MHz SPI clock
}