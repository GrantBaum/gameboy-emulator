void Interrupts::request(Type type){

} //turn that bit on in IF
void Interrupts::clear(Type type){

} //turn that bit off in IF
uint8_t Interrupts::pending(){

} //return IE & IF, only the 5 real bits
uint8_t Interrupts::read(uint16_t address){

} //FF0F is IF, FFFF is IE
void Interrupts::write(uint16_t address, uint8_t value){

}