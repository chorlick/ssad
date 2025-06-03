# 🛡️ Stealth Service Activation Daemon (SSAD)

The Stealth Service Activation Daemon is a secure C++ application that listens for a specific sequence of TCP connection attempts ("knocks") from a client. When the correct sequence is received in the correct order and timing, the daemon activates a hidden service and logs the activation. This allows for stealthy control over access to sensitive or internal services.

---

## Project Structure

```
├── C++ Take-Home Coding Challenge.pdf
├── Makefile                      # Makefile (try make all)
├── README.md                     # this file
├── configs                       # default sample configurations
│   ├── config.empty.json         # "bad" config, used for testing
│   └── config.json               # default config
│   ├── config.long-knock.json    # this configuration file for a "long knock timer"
│   └── config.out-of-order.json  # configuration file for knocking out of order
├── include                       # includees
│   ├── common.h                  # common header file (mainly used for the running flag to extern to other translation units)
│   ├── config.h                  # Loads and validates daemon configuration from a JSON file.
│   ├── logger.h                  # Thread-safe logger for writing timestamped log messages to a file.
│   ├── knockstate.h              # simple data strcuture used to track state of knocks as they come in
│   ├── ssad.h                    # Main orchestrator for the Stealth Service Activation Daemon (SSAD).
│   └── tracker.h                 # Tracks per-IP port knock sequences and triggers activation logging.
├── schemas                       # JSON schemas
│   └── config.schema.json        # Default schema
├── src                           # Source directory
│   ├── config.cpp                # Loads and validates daemon configuration from a JSON file.
│   ├── logger.cpp                # Thread-safe logger for writing timestamped log messages to a file.
│   ├── main.cpp                  # Main entry point here
│   ├── ssad.cpp                  # Main orchestrator for the Stealth Service Activation Daemon (SSAD).
│   └── tracker.cpp               # Tracks per-IP port knock sequences and triggers activation logging.
└── tools                         # Minor tools in python
    ├── knock-knock.py            # Simulates a TCP knock sequence to the daemon in python.
    └── schema_validator.py       # Script used to validate a configuration against the schema.
```
---

## Requirements

### C++ Dependencies
- Linux (Debian-based system)
- C++17 compatible compiler
- `nlohmann-json3-dev` (for JSON parsing)

Install with:
```bash
sudo apt install nlohmann-json3-dev
```

### Python Dependencies
- Python 3.8+
- `jsonschema` package

Install with:
```bash
pip install jsonschema
```

---

## Configuration File

### `configuration.json`
```json
{
  "trigger_ports": [10001, 10002, 10003],
  "sequence_timeout_ms": 5000,
  "inter_knock_timeout_ms": 1000,
  "activation_log_file": "activations.log"
}
```

---

## Build & Run

### Build the Daemon
```bash
cmake . 
make
```

### Run the Daemon
```bash
sudo ./ssad ./configs/config.json
```

Press `Ctrl+C` to shut down the daemon safely.

---

## Python Tools

### `knocker.py` - Send Knocks
Simulates a TCP knock sequence to the daemon.
```bash
python3 knocker.py 127.0.0.1 configuration.json -v
```

### `validate_config.py` - Validate Config JSON
Ensures your `configuration.json` matches the schema.
```bash
python3 validate_config.py schema/ssad_config.schema.json configuration.json
```

---

## Logging Output

- `/var/log/ssad_activations.log`: Logs everything here (except python output)

---

## Architecture Overview

### C++ Components

- **`Config`**: Loads and validates JSON configuration.
- **`LogFile`**: Thread-safe logger for error and activity logging.
- **`KnockTracker`**: Tracks knock sequences per-IP with timeouts.
- **`SSADaemon`**: Initializes config, sets up threads, and handles graceful shutdown.

### Concurrency & Safety

- Uses `std::thread` to listen on ports concurrently
- Uses `std::atomic<bool>` for shutdown signaling
- Uses `std::mutex` for safe access to log and state structures

---

## Security Considerations

- Port knocking is a form of security-through-obscurity and should not replace authentication.
- This implementation is for educational use and could be extended with:
  - IPv6 support
  - Integration with `iptables` or `firewalld`

---
