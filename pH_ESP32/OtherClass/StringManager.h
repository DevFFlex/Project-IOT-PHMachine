String intergerToText(int integer, int zeroforward){
  String zero = "";
  if (zeroforward <= 0 || integer < 0 || integer > 9) return String(integer);

  for (int i = 0; i < zeroforward - 1; i++) zero += "0";
  return zero + String(integer);
}



void splitString(String* databox, String text, String symbol, int size){
  char charArrayText[text.length() + 1];
  text.toCharArray(charArrayText, text.length() + 1);

  char charArraySymbol[symbol.length() + 1];
  symbol.toCharArray(charArraySymbol, symbol.length() + 1);

  char* symbol1 = charArraySymbol;

  char* part = strtok(charArrayText, symbol1);
  for (int i = 0; i < size; i++) {
    databox[i] = part;

    part = strtok(NULL, symbol1);
  }
}