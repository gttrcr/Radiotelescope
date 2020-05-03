float value(String *command)
{
  String value = command->substring(STRING_COMMAND_LENGTH, STRING_COMMAND_LENGTH + NUMBER_COMMAND_LENGTH);
  char valueArray[NUMBER_COMMAND_LENGTH + 1];
  value.toCharArray(valueArray, NUMBER_COMMAND_LENGTH + 1);
  return atof(valueArray);
}
