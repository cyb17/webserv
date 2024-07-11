import webbrowser

def open_pages(uri):
	chrome_path = '/usr/bin/google-chrome %s'
	webbrowser.get(chrome_path).open(uri)

if __name__ == "__main__":
	open_pages("localhost:1111/ressources/")