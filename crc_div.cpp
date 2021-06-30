/*
 * crc_div implementation can be inferred from it's call in main()
 * you can make this a header file and include, better keep headers and
 * source files separate
 * */

#include <iostream>
#include <cstring>

// xor two binary represented strings of equal length
const char* xor_strings(char str1[], char str2[], int str_length){
  char *xor_result = new char[str_length];
  for (int i = 0; i < str_length; i++){
    if (str1[i] == str2[i]){
      xor_result[i] = '0';
      continue;
    } 
    xor_result[i] = '1';
  }
  return xor_result;
}

// left shift a character array by 1, discard carry
void left_shift(char a[], int len){
  for (int i = 0; i < len-1; i++){
    a[i] = a[i+1];
  }
}

const char* pad_dividend(char dividend[], int dividend_len, int divisor_len){
  int new_dividend_length = dividend_len + divisor_len - 1;
  char *new_dividend = new char[new_dividend_length+1];

  strcpy(new_dividend, dividend);
  memset(new_dividend+dividend_len, '0', divisor_len-1);
  new_dividend[new_dividend_length] = '\0';

  return new_dividend;
}

// function returns only the crc remainder, concatenate later
const char* crc_div(char dividend[], char divisor[], 
    int dividend_len, int divisor_len){

  // pick is used to 'pick' up the next digit for division
  int pick = divisor_len;
  char *crc_rem = new char[divisor_len+1];
  const char *new_dividend = pad_dividend(dividend, dividend_len, divisor_len);

  // copy the initial divisor length string to crc_rem from dividend
  memcpy(crc_rem, new_dividend, divisor_len);

  while(pick < dividend_len){

    // if 1st but is 1, xor with dividend
    if (crc_rem[0] == '1'){
      const char* xor_result = xor_strings(divisor, crc_rem, divisor_len);
      // only take divisor_len -1 characters from offset 1 of xor_result
      // equivalent to assignment and shifting left at the same time
      strcpy(crc_rem, xor_result+1);
      delete xor_result;
    }

    // if 1st bit is 0, simply left shift
    else {
      left_shift(crc_rem, divisor_len);
    }

    // pick ==> bring down the next bit for division
    crc_rem[divisor_len-1] = new_dividend[pick];
    crc_rem[divisor_len] = '\0';
    pick++;
  }

  // for last n picked bit, separate from loop to prevent overflow
  if (crc_rem[0] == '1'){
    const char* xor_result = xor_strings(divisor, crc_rem, divisor_len);
    strcpy(crc_rem, xor_result);
    delete xor_result;
  }
  
  left_shift(crc_rem, divisor_len);
  crc_rem[divisor_len-1] = '\0';

  delete new_dividend;
  return crc_rem;
}

int main(){
  char dividend[] = "10111011"; 
  char divisor[] = "1001";
  const char* result = crc_div(dividend, divisor, 
      sizeof(dividend)-1, sizeof(divisor)-1);
  std::cout << result << std::endl; // only the crc remainder

  delete result;
  return 0;
}
