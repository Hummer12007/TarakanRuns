all: docs-en docs-ua

docs-en: Doxyfile-en
	doxygen Doxyfile-en

docs-ua: Doxyfile-ua
	doxygen Doxyfile-ua


clean: clean-docs

clean-docs:
	find . -maxdepth 1 -type d -name 'generated-*' -exec rm -r {} +
