from flask import Flask, request, Response
import requests

app = Flask(__name__)
TARGET_SERVER = "http://192.168.55.2:8080"

@app.route('/', defaults={'path': ''})
@app.route('/<path:path>', methods=['GET', 'POST', 'PUT', 'DELETE'])
def proxy(path):
    try:
        # Meneruskan request ke server target
        resp = requests.request(
            method=request.method,
            url=request.url.replace(request.host_url, TARGET_SERVER + '/'),
            headers={key: value for (key, value) in request.headers if key != 'Host'},
            data=request.get_data(),
            cookies=request.cookies,
            allow_redirects=False)

        # Membuat response untuk client
        excluded_headers = ['content-encoding', 'content-length', 'transfer-encoding', 'connection']
        headers = [(name, value) for (name, value) in resp.raw.headers.items()
                   if name.lower() not in excluded_headers]

        response = Response(resp.content, resp.status_code, headers)
        return response

    except requests.exceptions.RequestException as e:
        return str(e), 500

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=80)