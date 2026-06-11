"crypto_log.py"

import logging
from datetime import datetime

class CryptoLogger:
    """Sistema de logging centralizado para operaciones criptográficas"""
    _logger = None

    @classmethod
    def configurar_logger(cls):
        """Configura el sistema de logging para operaciones criptográficas"""
        if cls._logger is None:
            logging.basicConfig(
                level=logging.INFO,
                format='%(message)s'
            )
            cls._logger = logging.getLogger('CRYPTO')
        return cls._logger

    @staticmethod
    def log_crypto(operacion, detalles):
        """Registra una operación criptográfica con timestamp y detalles técnicos"""
        logger = CryptoLogger.configurar_logger()
        timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        logger.info(f"\n[CRYPTO] {timestamp} | {operacion}")
        for key, value in detalles.items():
            logger.info(f"  ├─ {key}: {value}")
        logger.info("  └─" + "─" * 60)
