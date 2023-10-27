/*// both D5 and D3 off = glide, both on = brake (if motor can do that)

void Foward(int(Speed)) {

  digitalWrite(D5, 1);    // set one direction
  digitalWrite(D3, 0);    // set one direction

  analogWrite(D4, Speed);   // go Value of Speed
  //delay(3000);

}

void Brake() {

#ifdef Use_Brake

  analogWrite(D4, 0);   // set Speed to Zero

  digitalWrite(D5, 1);    // set one direction
  digitalWrite(D3, 1);    // set one direction

#endif

}

void Reverse(int(Speed)) {

  digitalWrite(D5, 0);    // set one direction
  digitalWrite(D3, 1);    // set one direction

  analogWrite(D4, Speed);   // go Value of Speed

}

void Coast() {

  analogWrite(D4, 0);   // set Speed to Zero

  digitalWrite(D5, 0);    // set one direction
  digitalWrite(D3, 0);    // set one direction

}//*/
