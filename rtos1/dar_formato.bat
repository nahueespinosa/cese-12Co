
astyle --style=allman --recursive --add-brackets --align-reference=name --indent-classes --max-instatement-indent=90 --indent=spaces=4 --indent-classes --indent-switches --indent-col1-comments --max-instatement-indent=90 --pad-paren-in *.h 
astyle --style=allman --recursive --add-brackets --align-reference=name --indent-classes --max-instatement-indent=90 --indent=spaces=4 --indent-classes --indent-switches --indent-col1-comments --max-instatement-indent=90 --pad-paren-in *.c 
astyle --style=allman --recursive --add-brackets --align-reference=name --indent-classes --max-instatement-indent=90 --indent=spaces=4 --indent-classes --indent-switches --indent-col1-comments --max-instatement-indent=90 --pad-paren-in *.hpp 
astyle --style=allman --recursive --add-brackets --align-reference=name --indent-classes --max-instatement-indent=90 --indent=spaces=4 --indent-classes --indent-switches --indent-col1-comments --max-instatement-indent=90 --pad-paren-in *.cpp

del *.orig /s
del *.bak /s
