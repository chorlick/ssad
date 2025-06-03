import socket
import time
import json
import argparse
import logging
import sys

def load_config(path):
    try:
        with open(path, 'r') as f:
            return json.load(f)
    except Exception as e:
        logging.error(f"Failed to load configuration: {e}")
        sys.exit(1)

def knock(ip, ports, inter_knock_timeout_ms):
    for port in ports:
        try:
            logging.info(f"Knocking on {ip}:{port}")
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                s.settimeout(1)
                s.connect((ip, port))
        except Exception as e:
            logging.warning(f"Port {port} - Connection failed (expected): {e}")
        time.sleep( (inter_knock_timeout_ms - 100) / 1000.0)

def setup_logger(verbose: bool):
    logging.basicConfig(
        format='[%(asctime)s] %(levelname)s: %(message)s',
        datefmt='%Y-%m-%d %H:%M:%S',
        level=logging.DEBUG if verbose else logging.INFO
    )

def main():
    parser = argparse.ArgumentParser(description="Perform a TCP knock sequence on a target host.")
    parser.add_argument("ip", help="Target IP address")
    parser.add_argument("config", help="Path to JSON configuration file")
    parser.add_argument("-v", "--verbose", action="store_true", help="Enable verbose output to log")
    args = parser.parse_args()

    setup_logger(args.verbose)

    config = load_config(args.config)
    ports = config.get("trigger_ports", [])
    inter_timeout = config.get("inter_knock_timeout_ms", 1000)

    if not ports:
        logging.error("No trigger_ports defined in the configuration file.")
        sys.exit(1)

    knock(args.ip, ports, inter_timeout)

if __name__ == "__main__":
    main()