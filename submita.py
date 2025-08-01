import requests
import os
from totp import totp
import base64
from signature import create_pdf_signature
from dotenv import load_dotenv

load_dotenv()

TAHAP = 2
USERNAME = os.getenv('USERNAME')
FILE_PATH = f'A/{USERNAME}_A_{TAHAP}.pdf'
URL_MATH = os.getenv('MATH_URL')
URL_SUBMIT = os.getenv('SUBMIT_A_URL')
PRIVATE_KEY = os.getenv('PRIVATE_KEY_FILE')

if not os.path.exists(FILE_PATH):
  print(f"ERROR: File tidak ditemukan di {FILE_PATH}")
  exit()

def submita():
  response_math = requests.get(URL_MATH)
  
  if response_math.status_code != 200:
    print(f"Status: {response_math.status_code}, Response: {response_math.text}")
    return False

  data = response_math.json()
  question_string = data['question']
  math_problem = question_string.split('||')[0]
  math_answer = eval(math_problem)
  
  totp_code = totp()
  file_signature = create_pdf_signature(FILE_PATH, PRIVATE_KEY)
  signature_b64 = base64.b64encode(file_signature)

  with open(FILE_PATH, 'rb') as f:
    file_content = f.read()

  files_to_upload = {
    'file': (os.path.basename(FILE_PATH), file_content, 'application/pdf')
  }

  form_data = {
    "username": USERNAME,
    "totp_code": totp_code,
    "math_question": question_string,
    "math_answer": math_answer,
    "signature": signature_b64,
    "tahap": TAHAP,
  }

  print("Mengirim jawaban...")
  try:
    response_submit = requests.post(URL_SUBMIT, data=form_data, files=files_to_upload)

    print(f"Status Code Pengiriman: {response_submit.status_code}")
    print(f"Response Pengiriman: {response_submit.text}")

    return response_submit.status_code == 200

  except requests.exceptions.RequestException as e:
    print(f"Terjadi error saat request: {e}")
    return False

if __name__ == "__main__":
  submita()