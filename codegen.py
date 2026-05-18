#!/usr/bin/env python3
"""
Cinnabar to Python AST Code Generator
Преобразует AST Cinnabar в Python AST для выполнения в экосистеме Python
"""

import ast
import json
import sys
from typing import Any, List, Union


class CinnabarToPythonGenerator:
    """Генератор кода из Cinnabar AST в Python AST"""

    def __init__(self):
        self.variables = set()  # отслеживание объявленных переменных

    def generate(self, cinnabar_ast: dict) -> ast.Module:
        """Главная функция генерации"""
        if cinnabar_ast["kind"] != "Program":
            raise ValueError("Root node must be Program")

        body = []
        for stmt in cinnabar_ast["body"]:
            py_stmt = self.convert_stmt(stmt)
            if py_stmt is not None:
                if isinstance(py_stmt, list):
                    body.extend(py_stmt)
                else:
                    body.append(py_stmt)

        return ast.Module(body=body, type_ignores=[])

    def convert_stmt(self, node: dict) -> Union[ast.stmt, List[ast.stmt], None]:
        """Конвертация statement"""
        kind = node["kind"]

        if kind == "VarStmt":
            return self.convert_var_stmt(node)
        elif kind == "ExprStmt":
            return self.convert_expr_stmt(node)
        elif kind == "IfStmt":
            return self.convert_if_stmt(node)
        elif kind == "WhileStmt":
            return self.convert_while_stmt(node)
        elif kind == "Block":
            return self.convert_block(node)
        else:
            raise ValueError(f"Unknown statement kind: {kind}")

    def convert_var_stmt(self, node: dict) -> ast.Assign:
        """Конвертация объявления переменной в присваивание"""
        name = node["name"]
        self.variables.add(name)

        target = ast.Name(id=name, ctx=ast.Store())
        value = self.convert_expr(node["value"])

        return ast.Assign(targets=[target], value=value)

    def convert_expr_stmt(self, node: dict) -> ast.Expr:
        """Конвертация expression statement"""
        expr_node = node["expr"]

        # Если это AssignExpr, возвращаем Assign statement напрямую
        if expr_node["kind"] == "AssignExpr":
            return self.convert_assign_expr(expr_node)

        expr = self.convert_expr(expr_node)
        return ast.Expr(value=expr)

    def convert_if_stmt(self, node: dict) -> ast.If:
        """Конвертация if statement"""
        test = self.convert_expr(node["condition"])
        body = self.convert_block(node["then_branch"])

        orelse = []
        if "else_branch" in node and node["else_branch"]:
            orelse = self.convert_block(node["else_branch"])

        return ast.If(test=test, body=body, orelse=orelse)

    def convert_while_stmt(self, node: dict) -> ast.While:
        """Конвертация while statement"""
        test = self.convert_expr(node["condition"])
        body = self.convert_block(node["body"])

        return ast.While(test=test, body=body, orelse=[])

    def convert_block(self, node: dict) -> List[ast.stmt]:
        """Конвертация блока в список statements"""
        if node["kind"] != "Block":
            raise ValueError("Expected Block node")

        body = []
        for stmt in node["statements"]:
            py_stmt = self.convert_stmt(stmt)
            if py_stmt is not None:
                if isinstance(py_stmt, list):
                    body.extend(py_stmt)
                else:
                    body.append(py_stmt)

        return body if body else [ast.Pass()]

    def convert_expr(self, node: dict) -> ast.expr:
        """Конвертация expression"""
        kind = node["kind"]

        if kind == "Literal":
            return self.convert_literal(node)
        elif kind == "Identifier":
            return self.convert_identifier(node)
        elif kind == "BinaryExpr":
            return self.convert_binary_expr(node)
        elif kind == "UnaryExpr":
            return self.convert_unary_expr(node)
        elif kind == "AssignExpr":
            return self.convert_assign_expr(node)
        elif kind == "CallExpr":
            return self.convert_call_expr(node)
        else:
            raise ValueError(f"Unknown expression kind: {kind}")

    def convert_literal(self, node: dict) -> ast.Constant:
        """Конвертация литерала"""
        value = node["value"]
        return ast.Constant(value=value)

    def convert_identifier(self, node: dict) -> ast.Name:
        """Конвертация идентификатора"""
        name = node["name"]
        return ast.Name(id=name, ctx=ast.Load())

    def convert_binary_expr(self, node: dict) -> Union[ast.BinOp, ast.Compare, ast.BoolOp]:
        """Конвертация бинарного выражения"""
        op_str = node["op"]
        left = self.convert_expr(node["left"])
        right = self.convert_expr(node["right"])

        # Арифметические операции
        if op_str == "Add":
            return ast.BinOp(left=left, op=ast.Add(), right=right)
        elif op_str == "Sub":
            return ast.BinOp(left=left, op=ast.Sub(), right=right)
        elif op_str == "Mult":
            return ast.BinOp(left=left, op=ast.Mult(), right=right)
        elif op_str == "Div":
            return ast.BinOp(left=left, op=ast.Div(), right=right)
        elif op_str == "Pow":
            return ast.BinOp(left=left, op=ast.Pow(), right=right)

        # Операции сравнения
        elif op_str == "Eq":
            return ast.Compare(left=left, ops=[ast.Eq()], comparators=[right])
        elif op_str == "NotEq":
            return ast.Compare(left=left, ops=[ast.NotEq()], comparators=[right])
        elif op_str == "Lt":
            return ast.Compare(left=left, ops=[ast.Lt()], comparators=[right])
        elif op_str == "Gt":
            return ast.Compare(left=left, ops=[ast.Gt()], comparators=[right])
        elif op_str == "LtE":
            return ast.Compare(left=left, ops=[ast.LtE()], comparators=[right])
        elif op_str == "GtE":
            return ast.Compare(left=left, ops=[ast.GtE()], comparators=[right])

        # Логические операции
        elif op_str == "And":
            return ast.BoolOp(op=ast.And(), values=[left, right])
        elif op_str == "Or":
            return ast.BoolOp(op=ast.Or(), values=[left, right])

        else:
            raise ValueError(f"Unknown binary operator: {op_str}")

    def convert_unary_expr(self, node: dict) -> ast.UnaryOp:
        """Конвертация унарного выражения"""
        op_str = node["op"]
        operand = self.convert_expr(node["operand"])

        if op_str == "USub":
            return ast.UnaryOp(op=ast.USub(), operand=operand)
        elif op_str == "Not":
            return ast.UnaryOp(op=ast.Not(), operand=operand)
        else:
            raise ValueError(f"Unknown unary operator: {op_str}")

    def convert_assign_expr(self, node: dict) -> ast.Assign:
        """Конвертация присваивания"""
        target_node = node["target"]
        if target_node["kind"] != "Identifier":
            raise ValueError("Assignment target must be an identifier")

        target = ast.Name(id=target_node["name"], ctx=ast.Store())
        value = self.convert_expr(node["value"])

        return ast.Assign(targets=[target], value=value)

    def convert_call_expr(self, node: dict) -> ast.Call:
        """Конвертация вызова функции"""
        callee = self.convert_expr(node["callee"])
        args = [self.convert_expr(arg) for arg in node["args"]]

        return ast.Call(func=callee, args=args, keywords=[])


def compile_cinnabar_to_python(json_ast: str, output_file: str = None, execute: bool = False):
    """
    Компилирует Cinnabar AST (в JSON) в Python код

    Args:
        json_ast: JSON строка с AST
        output_file: путь для сохранения .py файла (опционально)
        execute: выполнить ли код после компиляции
    """
    # Парсим JSON
    cinnabar_ast = json.loads(json_ast)

    # Генерируем Python AST
    generator = CinnabarToPythonGenerator()
    python_ast = generator.generate(cinnabar_ast)

    # Фиксим отсутствующие location info
    ast.fix_missing_locations(python_ast)

    # Генерируем Python код
    python_code = ast.unparse(python_ast)

    # Сохраняем в файл если указан
    if output_file:
        with open(output_file, 'w', encoding='utf-8') as f:
            f.write(python_code)
        print(f"Generated Python code saved to: {output_file}")

    # Выполняем если нужно
    if execute:
        print("=" * 50)
        print("Executing generated Python code:")
        print("=" * 50)
        exec(compile(python_ast, filename="<cinnabar>", mode="exec"))

    return python_code


def main():
    if len(sys.argv) < 2:
        print("Usage: python codegen.py <ast.json> [--output output.py] [--execute]")
        sys.exit(1)

    json_file = sys.argv[1]
    output_file = None
    execute = False

    # Парсим аргументы
    i = 2
    while i < len(sys.argv):
        if sys.argv[i] == "--output" and i + 1 < len(sys.argv):
            output_file = sys.argv[i + 1]
            i += 2
        elif sys.argv[i] == "--execute":
            execute = True
            i += 1
        else:
            i += 1

    # Читаем JSON AST
    with open(json_file, 'r', encoding='utf-8') as f:
        json_ast = f.read()

    # Компилируем
    python_code = compile_cinnabar_to_python(json_ast, output_file, execute)

    # Выводим код если не сохраняем в файл
    if not output_file:
        print(python_code)


if __name__ == "__main__":
    main()
