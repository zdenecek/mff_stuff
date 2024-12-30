
from enum import Enum
import json
import luigi
import luigi.contrib.hadoop
import luigi.contrib.hdfs
from luigi.mock import MockTarget
import radb
import radb.ast
import radb.parse

'''
Control where the input data comes from, and where output data should go.
'''
class ExecEnv(Enum):
    LOCAL = 1   # read/write local files
    HDFS = 2    # read/write HDFS
    MOCK = 3    # read/write mock data to an in-memory file system.

'''
Switches between different execution environments and file systems.
'''
class OutputMixin(luigi.Task):
    exec_environment = luigi.EnumParameter(enum=ExecEnv, default=ExecEnv.HDFS)
    
    def get_output(self, fn):
        if self.exec_environment == ExecEnv.HDFS:
            return luigi.contrib.hdfs.HdfsTarget(fn)
        elif self.exec_environment == ExecEnv.MOCK:
            return MockTarget(fn)
        else:
            return luigi.LocalTarget(fn)


class InputData(OutputMixin):
    filename = luigi.Parameter()

    def output(self):
        return self.get_output(self.filename)


'''
Counts the number of steps / luigi tasks that we need for evaluating this query.
'''
def count_steps(raquery):
    assert(isinstance(raquery, radb.ast.Node))

    if (isinstance(raquery, radb.ast.Select) or isinstance(raquery,radb.ast.Project) or
        isinstance(raquery,radb.ast.Rename)):
        return 1 + count_steps(raquery.inputs[0])

    elif isinstance(raquery, radb.ast.Join):
        return 1 + count_steps(raquery.inputs[0]) + count_steps(raquery.inputs[1])

    elif isinstance(raquery, radb.ast.RelRef):
        return 1

    else:
        raise Exception("count_steps: Cannot handle operator " + str(type(raquery)) + ".")


class RelAlgQueryTask(luigi.contrib.hadoop.JobTask, OutputMixin):
    '''
    Each physical operator knows its (partial) query string.
    As a string, the value of this parameter can be searialized
    and shipped to the data node in the Hadoop cluster.
    '''
    querystring = luigi.Parameter()

    '''
    Each physical operator within a query has its own step-id.
    This is used to rename the temporary files for exhanging
    data between chained MapReduce jobs.
    '''
    step = luigi.IntParameter(default=1)

    '''
    In HDFS, we call the folders for temporary data tmp1, tmp2, ...
    In the local or mock file system, we call the files tmp1.tmp...
    '''
    def output(self):
        if self.exec_environment == ExecEnv.HDFS:
            filename = "tmp" + str(self.step)
        else:
            filename = "tmp" + str(self.step) + ".tmp"
        return self.get_output(filename)


'''
Given the radb-string representation of a relational algebra query,
this produces a tree of luigi tasks with the physical query operators.
'''
def task_factory(raquery, step=1, env=ExecEnv.HDFS):
    assert(isinstance(raquery, radb.ast.Node))
    
    if isinstance(raquery, radb.ast.Select):
        return SelectTask(querystring=str(raquery) + ";", step=step, exec_environment=env)

    elif isinstance(raquery, radb.ast.RelRef):
        filename = raquery.rel + ".json"
        return InputData(filename=filename, exec_environment=env)

    elif isinstance(raquery, radb.ast.Join):
        return JoinTask(querystring=str(raquery) + ";", step=step, exec_environment=env)

    elif isinstance(raquery, radb.ast.Project):
        return ProjectTask(querystring=str(raquery) + ";", step=step, exec_environment=env)

    elif isinstance(raquery, radb.ast.Rename):
        return RenameTask(querystring=str(raquery) + ";", step=step, exec_environment=env)
                          
    else:
        # We will not evaluate the Cross product on Hadoop, too expensive.
        raise Exception("Operator " + str(type(raquery)) + " not implemented (yet).")
    

class JoinTask(RelAlgQueryTask):

    def requires(self):
        raquery = radb.parse.one_statement_from_string(self.querystring)
        assert(isinstance(raquery, radb.ast.Join))
      
        task1 = task_factory(raquery.inputs[0], step=self.step + 1, env=self.exec_environment)
        task2 = task_factory(raquery.inputs[1], step=self.step + count_steps(raquery.inputs[0]) + 1, env=self.exec_environment)

        return [task1, task2]

    
    def mapper(self, line):
        relation, tuple = line.split('\t')
        json_tuple = json.loads(tuple)
        
        raquery = radb.parse.one_statement_from_string(self.querystring)
        condition = raquery.cond

        ''' ...................... fill in your code below ........................'''

        attr_ref_0 = condition.inputs[0]
        attr_ref_1 = condition.inputs[1]

        if isinstance(attr_ref_0, radb.ast.AttrRef) and isinstance(attr_ref_1, radb.ast.AttrRef):
            if attr_ref_0.name == attr_ref_1.name:
                key = f"{relation}.{attr_ref_0.name}"
                value = (relation, json_tuple)
                yield (json_tuple[key], value)
        else:
            # Check if both inputs are binary operations
            binary_op_0 = condition.inputs[0]
            binary_op_1 = condition.inputs[1]
            
            if (isinstance(binary_op_0, radb.ast.ValExprBinaryOp) and 
                isinstance(binary_op_1, radb.ast.ValExprBinaryOp)):
                
                left_name_0 = binary_op_0.inputs[0].name
                right_name_0 = binary_op_0.inputs[1].name
                left_name_1 = binary_op_1.inputs[0].name
                right_name_1 = binary_op_1.inputs[1].name
                
                if left_name_0 == right_name_0 and left_name_1 == right_name_1:
                    key_0 = f"{relation}.{left_name_0}"
                    key_1 = f"{relation}.{left_name_1}"
                    values = [json_tuple[key_0], json_tuple[key_1]]
                    yield (values, (relation, json_tuple))

        ''' ...................... fill in your code above ........................'''


    def reducer(self, key, values):
        raquery = radb.parse.one_statement_from_string(self.querystring)
        
        ''' ...................... fill in your code below ........................'''

        condition = raquery.cond

        if isinstance(key, list):
            rel1 = condition.inputs[0].inputs[0].rel
            rel2 = condition.inputs[1].inputs[1].rel
        else:
            rel1 = condition.inputs[0].rel
            rel2 = condition.inputs[1].rel

        sorted_values = sorted(values, key=lambda x: x[0])

        relations = [rel for rel, _ in sorted_values]

        max_count = relations.count(rel1) * relations.count(rel2)

        solution = {}
        cnt = 1

        for current_rel, current_dict in sorted_values:
            solution.update(current_dict)
            for other_rel, other_dict in sorted_values:
                if current_rel != other_rel and cnt <= max_count:
                    solution.update(other_dict)
                    cnt += 1
                    yield (current_rel, json.dumps(solution))

        ''' ...................... fill in your code above ........................'''   


class SelectTask(RelAlgQueryTask):

    def requires(self):
        raquery = radb.parse.one_statement_from_string(self.querystring)
        assert(isinstance(raquery, radb.ast.Select))
        
        return [task_factory(raquery.inputs[0], step=self.step + 1, env=self.exec_environment)]

    
    def mapper(self, line):
        relation, tuple = line.split('\t')
        json_tuple = json.loads(tuple)

        condition = radb.parse.one_statement_from_string(self.querystring).cond
        
        ''' ...................... fill in your code below ........................'''

        if not isinstance(condition.inputs[0], radb.ast.AttrRef) and not isinstance(condition.inputs[0], radb.ast.ValExprBinaryOp):
            condition.inputs[0], condition.inputs[1] = condition.inputs[1], condition.inputs[0]

        if isinstance(condition, radb.ast.ValExprBinaryOp) and isinstance(condition.inputs[0], radb.ast.AttrRef):
            for key, value in json_tuple.items():
                if isinstance(condition.inputs[1], radb.ast.Literal) and str(condition.inputs[0].name) in key:
                    literal_value = str(condition.inputs[1].val).replace('\'', '')
                    if literal_value == str(value):
                        yield (relation, json.dumps(json_tuple))
        else:
            key_1 = f"{relation}.{condition.inputs[0].inputs[0].name}"
            key_2 = f"{relation}.{condition.inputs[1].inputs[0].name}"

            if key_1 in json_tuple and key_2 in json_tuple:
                value_1 = str(condition.inputs[0].inputs[1].val).replace('\'', '')
                value_2 = str(condition.inputs[1].inputs[1].val).replace('\'', '')

                if value_1 in json_tuple.values() and float(value_2) in json_tuple.values():
                    yield (relation, json.dumps(json_tuple))

        
        ''' ...................... fill in your code above ........................'''


class RenameTask(RelAlgQueryTask):

    def requires(self):
        raquery = radb.parse.one_statement_from_string(self.querystring)
        assert(isinstance(raquery, radb.ast.Rename))

        return [task_factory(raquery.inputs[0], step=self.step + 1, env=self.exec_environment)]


    def mapper(self, line):
        relation, tuple = line.split('\t')
        json_tuple = json.loads(tuple)

        raquery = radb.parse.one_statement_from_string(self.querystring)
        
        ''' ...................... fill in your code below ........................'''

        def get(q):
   
            if q is None: 
                return None
            if isinstance(q, radb.ast.Rename):
                return q
            if isinstance(q, radb.ast.Select):
                val = get(q.cond)
                if val is not None:
                    return val
            for item in q.inputs:
                val = get(item)
                if val is not None:
                    return val
        first_rename = get(raquery)
        res = {str(key).replace(relation, first_rename.relname): val for key, val in json_tuple.items() }
        yield (first_rename.relname, json.dumps(res))
        
        ''' ...................... fill in your code above ........................'''


class ProjectTask(RelAlgQueryTask):

    def requires(self):
        raquery = radb.parse.one_statement_from_string(self.querystring)
        assert(isinstance(raquery, radb.ast.Project))

        return [task_factory(raquery.inputs[0], step=self.step + 1, env=self.exec_environment)]    


    def mapper(self, line):
        relation, tuple = line.split('\t')
        json_tuple = json.loads(tuple)

        attrs = radb.parse.one_statement_from_string(self.querystring).attrs

        ''' ...................... fill in your code below ........................'''

              
        for element_key in json_tuple:
            attribute_map = {}
            for element_key in json_tuple:
                for attribute in attrs:
                    if not isinstance(attribute, radb.ast.AttrRef):
                        continue
                    if str(attribute.name) in str(element_key) and str(element_key) not in attribute_map:
                        attribute_map[element_key] = json_tuple[element_key]
            
            yield (json.dumps(attribute_map), json.dumps(attribute_map))

        
        ''' ...................... fill in your code above ........................'''


    def reducer(self, key, values):

        ''' ...................... fill in your code below ........................'''
        visited = set()
        for val in values:
            if val in visited:
                continue
            visited.add(val)
            yield (key, [val])
        ''' ...................... fill in your code above ........................'''
        

if __name__ == '__main__':
    luigi.run()
