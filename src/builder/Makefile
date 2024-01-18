PYTHON = python

all: link_install

link_install:
	${PYTHON} -m pip install --break-system-packages -e .

pip_upload:
	# sudo apt install python3-venv
	# ${PYTHON} -m pip install --upgrade --break-system-packages twine
	${PYTHON} -m build
	${PYTHON} -m twine upload --repository vfs_build --verbose dist/*


.PHONY: \
	link_install \
	pip_upload \
	all