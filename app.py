from flask import Flask, render_template, request, jsonify
import subprocess
import os

app = Flask(__name__)
app.config['SECRET_KEY'] = 'your_basic_secret_key_for_flash'

C_FILES = ["main.c", "auth.c", "product.c", "cart.c", "fraud.c"] 
C_EXECUTABLE = "./smartecom"

def compile_c_code():
    """Compiles the modular C backend."""
    compile_command = ["gcc"] + C_FILES + ["-o", C_EXECUTABLE]
    try:
        subprocess.run(compile_command, check=True, capture_output=True, text=True)
        return True, "C backend compiled successfully!"
    except subprocess.CalledProcessError as e:
        error_msg = f"Compilation failed! GCC Output:\n{e.stderr}"
        return False, error_msg
    except FileNotFoundError:
        return False, "GCC compiler not found. Please ensure it is installed."

def run_c_backend(action, *params):
    """Runs the compiled C executable with a given action and parameters."""
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
        # Split the output by line and remove empty lines
        output_lines = [line for line in result.stdout.strip().split('\n') if line]
        return output_lines, True
    
    except subprocess.CalledProcessError as e:
        error_output = [f"C Runtime Error (Action: {action})"]
        error_output.extend(e.stderr.strip().split('\n'))
        return error_output, False
    
    except Exception as e:
        return [f"Fatal Error running C code: {e}"], False

# --- Web Routes ---

# AJAX route for fast, reliable authentication (returns JSON status)
@app.route("/auth", methods=["POST"])
def auth_api():
    action = request.form.get("action")
    username = request.form.get("username", "")
    password = request.form.get("password", "")
    
    output_lines, success = run_c_backend(action, username, password)

    # Crucial Fix: Check for the unique success tag returned by main.c
    auth_success = any("AUTH_SUCCESS" in line for line in output_lines)

    return jsonify({
        "status": "success" if auth_success else "failure",
        "output": output_lines
    })


# Main route (handles GET and all other POST actions like SEARCH, CART_ADD)
@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":
        action = request.form.get("action")
        param1 = request.form.get("param1", "")
        param2 = request.form.get("param2", "")
        
        # All non-auth actions still return the full HTML template update
        output_lines, success = run_c_backend(action, param1, param2)
        
        return render_template("index.html", 
                               output_lines=output_lines,
                               action=action, 
                               param1=param1,
                               param2=param2,
                               success=success)
    
    # Run a quick check/setup on first load
    compile_success, compile_message = compile_c_code()
    initial_output = ["Welcome to the SmartEcom DSA Engine Demo!"]
    if not compile_success:
        initial_output.append(f"!!! CRITICAL: {compile_message} !!!")
    else:
        initial_output.append("C Backend is compiled and ready.")
        
    # Start on the login screen
    return render_template("index.html", initial_output=initial_output, success=compile_success)

@app.route("/execute_cart_undo", methods=["POST"])
def execute_cart_undo():
    # Dedicated route for CART_UNDO
    output_lines, success = run_c_backend("CART_UNDO")
    return jsonify({"output": output_lines, "success": success})

# --- Run App ---
if __name__ == "__main__":
    compile_success, msg = compile_c_code()
    if compile_success:
        print("Starting Flask app...")
        app.run(debug=True)
    else:
        print(f"Flask did not start due to C compilation error: {msg}")