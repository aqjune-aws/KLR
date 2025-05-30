/-
Copyright (c) 2025 Amazon.com, Inc. or its affiliates. All Rights Reserved.
Released under Apache 2.0 license as described in the file LICENSE.
Authors: Paul Govereau, Sean McLaughlin
-/
import Lean

open Lean(Syntax TSyntax mkIdent)
open Lean.Elab.Command(CommandElab elabCommand)
open Lean.Parser.Term(matchAltExpr)

syntax item := Lean.Parser.Command.ctor ":=" num
syntax items := manyIndent(item)
syntax (name := enumcmd) "private"? "enum" ident "where" items : command

namespace KLR.Util

class Enum (a : Type) where
  toUInt8 : a -> UInt8
  fromUInt8 : UInt8 -> Except String a

private def hasDuplicate [BEq a] (xs : List a) : Option a := Id.run do
  let mut ys := []
  for x in xs do
    if ys.contains x then return (some x)
    ys := x :: ys
  return none

@[command_elab enumcmd]
def elabEnum : CommandElab
  | `(enum $name where $items*) => doit false name items
  | `(private enum $name where $items*) => doit true name items
  | _ => throwError "invalid enum syntax"
where
  doit priv name items := do
    let mut values : List (TSyntax `Lean.Parser.Command.ctor × TSyntax `num) := []
    let mut res : Array (TSyntax `Lean.Parser.Command.ctor) := #[]
    for ctor in items do
      match ctor with
      | `(item| $id := $v) => do
        values := (id, v) :: values
        res := res.push id
      | e => throwError s!"invalid enum item {e}"
    res := res.reverse
    let nums := (values.map fun (_, n) => n.getNat)
    match hasDuplicate nums with
    | some k =>
      throwError s!"Duplicate numeric values: {k}"
    | none =>
    let cmd <- if priv then
      `(
        private inductive $name where
          $[ $res ]*
      )
    else
      `(
        inductive $name where
          $[ $res ]*
      )
    elabCommand cmd
    let toUInt8Name := mkIdent (.str name.raw.getId "toUInt8")
    let mut cases : Array (TSyntax `Lean.Parser.Term.matchAltExpr) := #[]
    for (c, n) in values do
      -- TODO: How should I actually turn a ctor to a term?
      -- Sketchy to grab the 3rd field, but I saw this in dbg_trace:
      --     Command.ctor [] "|" (Command.declModifiers [] [] [] [] [] []) `r (Command.optDeclSig [] []))
      let c := mkIdent (c.raw.getIdAt 3)
      let case <- `(matchAltExpr| | $c => $n:num)
      cases := cases.push case
    let toUInt8 <- `(
      private def $toUInt8Name:ident (x : $name) : UInt8 := match x with
        $cases:matchAlt*
    )

    elabCommand toUInt8
    let fromUInt8Name := mkIdent (.str name.raw.getId "fromUInt8")
    let mut cases1 : Array (TSyntax `Lean.Parser.Term.matchAltExpr) := #[]
    for (c, n) in values do
      let c := mkIdent (c.raw.getIdAt 3)
      let case <- `(matchAltExpr| | $n:num => return .$c)
      cases1 := cases1.push case
    let other <- `(matchAltExpr| | n => throw s!"Unexpected numeric code: {n}")
    cases1 := cases1.push other
    let fromUInt8 <- `(
      private def $fromUInt8Name:ident (x : UInt8) : Except String $name := match x with
        $cases1:matchAlt*
    )
    elabCommand fromUInt8
    let instances <- `(
      deriving instance BEq, Inhabited, Repr for $name
    )
    elabCommand instances
    let fromUInt8!Name : Lean.Ident := mkIdent (.str name.raw.getId "fromUInt8!")
    let n : Lean.Ident := Lean.mkIdent (.str .anonymous "n")
    let app : Lean.Term := Syntax.mkApp fromUInt8Name #[n]
    let fromUInt8! <- `(
      def $fromUInt8!Name:ident ($n : UInt8) : $name := match $app:term with
      | .ok v => v
      | .error msg => panic msg
    )
    elabCommand fromUInt8!
    let inst <- `(
      instance : Enum $name where
        toUInt8 := $toUInt8Name
        fromUInt8 := $fromUInt8Name
    )
    elabCommand inst

section Test

private enum Foo where
  | x := 4
  | y := 6
  | z := 5
  | q := 0
  | r := 9
  | m := 10

#guard Foo.x.toUInt8 == 4
#guard Foo.fromUInt8! Foo.x.toUInt8 == Foo.x
#guard Foo.fromUInt8! Foo.m.toUInt8 == Foo.m

end Test

end KLR.Util
