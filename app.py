from flask import Flask, render_template, request, jsonify
import subprocess
import os

app = Flask(__name__)
app.config['SECRET_KEY'] = 'your_basic_secret_key_for_flash'

SRC_DIR = "src"
INCLUDE_DIR = "include"
BIN_DIR = "bin"

C_FILES = [
    os.path.join(SRC_DIR, "main.c"),
    os.path.join(SRC_DIR, "auth.c"),
    os.path.join(SRC_DIR, "product.c"),
    os.path.join(SRC_DIR, "cart.c"),
    os.path.join(SRC_DIR, "fraud.c"),
    os.path.join(SRC_DIR, "order.c")
]
C_EXECUTABLE = os.path.join(BIN_DIR, "smartecom.exe")

def compile_c_code():
    os.makedirs(BIN_DIR, exist_ok=True)
    compile_command = ["gcc", "-I", INCLUDE_DIR] + C_FILES + ["-o", C_EXECUTABLE]
    try:
        subprocess.run(compile_command, check=True, capture_output=True, text=True)
        return True, "C backend compiled successfully!"
    except subprocess.CalledProcessError as e:
        error_msg = f"Compilation failed! GCC Output:\n{e.stderr}"
        return False, error_msg
    except FileNotFoundError:
        return False, "GCC compiler not found. Please ensure it is installed."

def run_c_backend(action, *params):
    if not os.path.exists(C_EXECUTABLE):
        return ["ERROR: C executable not found. Please compile first."], False

    command = [C_EXECUTABLE, action] + list(params)

    try:
        result = subprocess.run(
            command,
            capture_output=True,
            text=True,
            check=True
        )
        output_lines = [line for line in result.stdout.strip().split('\n') if line]
        return output_lines, True

    except subprocess.CalledProcessError as e:
        error_output = [f"C Runtime Error (Action: {action})"]
        error_output.extend(e.stderr.strip().split('\n'))
        return error_output, False

    except Exception as e:
        return [f"Fatal Error running C code: {e}"], False

@app.route("/auth", methods=["POST"])
def auth_api():
    action = request.form.get("action")
    username = request.form.get("username", "")
    password = request.form.get("password", "")

    output_lines, success = run_c_backend(action, username, password)

    auth_success = any("AUTH_SUCCESS" in line for line in output_lines)

    return jsonify({
        "status": "success" if auth_success else "failure",
        "output": output_lines
    })

@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":
        action = request.form.get("action")
        param1 = request.form.get("param1", "")
        param2 = request.form.get("param2", "")

        output_lines, success = run_c_backend(action, param1, param2)

        return render_template("index.html",
                               output_lines=output_lines,
                               action=action,
                               param1=param1,
                               param2=param2,
                               success=success)

    compile_success, compile_message = compile_c_code()
    initial_output = ["Welcome to the SmartEcom DSA Engine Demo!"]
    if not compile_success:
        initial_output.append(f"!!! CRITICAL: {compile_message} !!!")
    else:
        initial_output.append("C Backend is compiled and ready.")

    return render_template("index.html", initial_output=initial_output, success=compile_success)

@app.route("/execute_cart_undo", methods=["POST"])
def execute_cart_undo():
    output_lines, success = run_c_backend("CART_UNDO")
    return jsonify({"output": output_lines, "success": success})

if __name__ == "__main__":
    compile_success, msg = compile_c_code()
    if compile_success:
        print("Starting Flask app...")
        app.run(debug=True)
    else:
        print(f"Flask did not start due to C compilation error: {msg}")