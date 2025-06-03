import json
import argparse
import logging
from jsonschema import validate, ValidationError

def load_json(path):
    with open(path, 'r') as f:
        return json.load(f)

def validate_config(schema_path, config_path):
    schema = load_json(schema_path)
    config = load_json(config_path)

    try:
        validate(instance=config, schema=schema)
        logging.info("Validation passed.")
        return True
    except ValidationError as e:
        logging.error(f"Validation failed: {e.message}")
        return False

def main():
    parser = argparse.ArgumentParser(description="validate ssad configuration against schema")
    parser.add_argument("schema", help="path to json Schema file")
    parser.add_argument("config", help="path to confguration json file")
    args = parser.parse_args()

    logging.basicConfig(level=logging.INFO, format="[%(asctime)s] %(levelname)s: %(message)s")

    success = validate_config(args.schema, args.config)
    exit(0 if success else 1)

if __name__ == "__main__":
    main()